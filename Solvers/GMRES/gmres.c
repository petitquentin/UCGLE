/*Copyright (c) 2011—2017. Pierre-Yves AQUILANTI and Xinzhe WU in Maison de la Simulation. All rights reserved */
#include "gmres.h"

PetscErrorCode launchGMRES(com_lsa * com, Vec * b, Mat * A){
	PetscErrorCode ierr;
	KSP ksp, kspp;
	Vec x, c;
	KSPConvergedReason reason;
	PetscInt its, nols, ntimes;
	int i, size;
	PetscBool flagls, flagtimes, gft_flg, guess_flg;
	double cost_time;
	clock_t start, end;


	KSP_FGMRES     *fgmres = (KSP_FGMRES *)(ksp->data);


	PetscReal norm;
	PetscOptionsHasName(NULL,NULL,"-GMRES_FT",&gft_flg);
	
	PetscOptionsHasName(NULL,NULL,"-initial_guess_seq_rhs", &guess_flg);
	
	VecGetSize(*b, &size);
	VecDuplicate(*b, &c);
        PetscPrintf(com->com_group,"#} GMRES Creating and setting vector x\n");
	ierr = VecDuplicate(*b, &x);CHKERRQ(ierr);
	ierr = PetscObjectSetName((PetscObject) x, "Solution");CHKERRQ(ierr);
	ierr = VecSet(x, 0.0); CHKERRQ(ierr);
	ierr = PetscOptionsGetInt(NULL,PETSC_NULL,"-ksp_ls_nopc",&nols,&flagls);CHKERRQ(ierr);
	ierr = KSPCreate(com->com_group, &ksp);CHKERRQ(ierr);
	ierr = KSPSetType(ksp,KSPFGMRES);CHKERRQ(ierr);
//	KSPSetInitialGuessNonzero(ksp, PETSC_TRUE);
	ierr = KSPSetOperators(ksp, *A, *A);CHKERRQ(ierr);
	ierr = KSPSetFromOptions(ksp);CHKERRQ(ierr);
//        KSPSetInitialGuessNonzero(ksp, PETSC_TRUE);
	PetscOptionsGetInt(NULL,NULL,"-ntimes",&ntimes,&flagtimes);

	if(!flagtimes){
		ntimes=1;
	}

	for (i=1; i<=ntimes; i++) {
		mpi_lsa_com_type_send(com,&i);

                        VecCopy(*b,c);
                        VecNorm(c, NORM_2,&norm);
                        VecScale(c, 1/norm);


//                        VecView(c,PETSC_VIEWER_STDOUT_WORLD);

/*
		if(i==1){
			VecCopy(*b,c);
			VecNorm(c, NORM_2,&norm);
			VecScale(c, 0.01/norm);
		}
		else{
//	                        VecCopy(*b,c);
			generate_random_seed_vector(size, -10,10, i,&c);
			VecNorm(c, NORM_2,&norm);
			VecScale(c, 0.01/norm);
		}
*/

        if (i == 1){

        	start=clock();
			PetscPrintf(PETSC_COMM_WORLD,"\n\n------------\nStart the %d times Resolution \n-------------\n\n",i);
			ierr = MyKSPSolve(ksp, c, x,com); CHKERRQ(ierr);
			end=clock();
  			cost_time = (double)(end - start)/CLOCKS_PER_SEC;
			KSPGetConvergedReason(ksp,&reason);
			if (reason<0 && !gft_flg) {
				PetscPrintf(PETSC_COMM_WORLD,"\nResolution %d: Divergence in acceptable iteration steps.\n", i);
			}
			else if(reason < 0 && gft_flg){
				int gft = 999;
				mpi_lsa_com_type_send(com,&gft);
				PetscPrintf(PETSC_COMM_WORLD, "\n\n\nERROR >>> GMRES Simulated to be failure, so send the signal 999 \n");
			}
			else {
				KSPGetIterationNumber(ksp,&its);
		//	mpi_lsa_com_type_send(com,&i);
				PetscPrintf(PETSC_COMM_WORLD,"\nResolution %d: Convergence in %f seconds / %d iterations. \n", i, cost_time, its);
		
			}
        }

        else{
			VecCopy(*b,c);
			generate_random_seed_vector(size, -10,10, i,&c);
			VecNorm(c, NORM_2,&norm);
			VecScale(c, 1/norm);
		
			start=clock();

		if(guess_flg){

			VecCopy(c, ksp->vec_rhs);
			KSPSetInitialGuessNonzero(ksp, PETSC_FALSE);

			if(!LSInitialGuess(com,ksp))
    		{
	   			PetscPrintf(PETSC_COMM_WORLD,"\n@@@>>>Generating the initial guess vector using LS method based previous resolution\n\n",ksp->its);
    		}

    		VecCopy(ksp->vec_sol, x);
    		KSPSetInitialGuessNonzero(ksp, PETSC_TRUE);
		}


			PetscPrintf(PETSC_COMM_WORLD,"\n\n------------\nStart the %d times Resolution \n-------------\n\n",i);
			ierr = MyKSPSolve(ksp, c, x,com); CHKERRQ(ierr);
			end=clock();
  			cost_time = (double)(end - start)/CLOCKS_PER_SEC;
			KSPGetConvergedReason(ksp,&reason);
			Vec tmp2;
                	VecDuplicate(x,&tmp2);
			MatMult(*A,x,tmp2);
                	VecAXPY(tmp2,-1,c);
	                PetscReal nvt2;
	                VecNorm(tmp2, NORM_2, &nvt2);

			if (reason<0 && !gft_flg) {
				PetscPrintf(PETSC_COMM_WORLD,"\nResolution %d: Divergence in acceptable iteration steps.\n", i);
			}
			else if(reason < 0 && gft_flg){
				int gft = 999;
				mpi_lsa_com_type_send(com,&gft);
				PetscPrintf(PETSC_COMM_WORLD, "\n\n\nERROR >>> GMRES Simulated to be failure, so send the signal 999 \n");
			}
			else {
				KSPGetIterationNumber(ksp,&its);
		//	mpi_lsa_com_type_send(com,&i);
				PetscPrintf(PETSC_COMM_WORLD,"\nResolution %d: Convergence in %f seconds / %d iterations and residual = %.12f\n", i, cost_time, its, nvt2);
		
			}

        }

//		VecView(ksp->vec_rhs,PETSC_VIEWER_STDOUT_WORLD);
	}

			int exit_type=666;
                        mpi_lsa_com_type_send(com,&exit_type);
                        PetscPrintf(com->com_group,"\n\n#}Finish the resolution\n");

  	return ierr;
}
