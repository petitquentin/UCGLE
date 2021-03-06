/*Copyright (c) 2011—2017. Pierre-Yves AQUILANTI and Xinzhe WU in Maison de la Simulation. All rights reserved */
#include "real2complex.h"



PetscErrorCode real2complexMat(Mat *A,Mat * r2c){
	PetscErrorCode ierr;
	Mat C;
	Mat R;
	PetscInt sizerow,sizecol;
	PetscInt ncols,*cols_p,icol;
	const PetscInt * cols;
	const PetscScalar * vals;
	PetscScalar *arrayR, *arrayC,*values;
	int i,j;

	// copy of the matrix
	ierr=MatDuplicate(*A,MAT_COPY_VALUES,&C);CHKERRQ(ierr);
	ierr=MatDuplicate(*A,MAT_COPY_VALUES,&R);CHKERRQ(ierr);

	ierr=MatRealPart(R);CHKERRQ(ierr);
	ierr=MatImaginaryPart(C);CHKERRQ(ierr);

	// The values of the two matrices are recovered in tables
	ierr=MatGetSize(*A,&sizerow,&sizecol);CHKERRQ(ierr);
	ierr=PetscMalloc(sizerow*sizecol*sizeof(PetscScalar),&arrayR);CHKERRQ(ierr);
	ierr=PetscMalloc(sizerow*sizecol*sizeof(PetscScalar),&arrayC);CHKERRQ(ierr);
	ierr=MatCreate(PETSC_COMM_WORLD,r2c);CHKERRQ(ierr);
	ierr=MatSetSizes(*r2c,PETSC_DECIDE,PETSC_DECIDE,sizerow*sizerow,sizecol*sizecol);CHKERRQ(ierr);
	ierr=MatSetFromOptions(*r2c);CHKERRQ(ierr);

	for(i=0;i<sizerow;i++){
		// for real
		ierr=MatGetRow(R,i,&ncols,&cols,&vals);CHKERRQ(ierr);
 		ierr=PetscMalloc(sizeof(PetscScalar)*(ncols+1),&values);CHKERRQ(ierr);
		ierr=PetscMalloc(sizeof(PetscScalar)*(ncols),&cols_p);CHKERRQ(ierr);
 		ierr=PetscMemcpy(values,vals,ncols);CHKERRQ(ierr);
		ierr=PetscMemcpy(cols_p,cols,ncols);CHKERRQ(ierr);

		// set first imaginary values part first shift
		ierr=MatSetValues(*r2c,1,&i,ncols,cols,vals,INSERT_VALUES);CHKERRQ(ierr);

		//  invert them
		for(j=0;j<ncols;j++){
			values[j]=-values[j];
			cols_p+=sizecol;
		}
		// and set again with shift of sizerow
		icol=i+sizerow;
		ierr=MatSetValues(*r2c,1,&icol,*cols_p,cols,vals,INSERT_VALUES);CHKERRQ(ierr);
		ierr=PetscFree(values);CHKERRQ(ierr);
		ierr=PetscFree(cols_p);CHKERRQ(ierr);
		// for imaginary
		ierr=MatGetRow(R,i,&ncols,&cols,&vals);CHKERRQ(ierr);
 		ierr=PetscMalloc(sizeof(PetscScalar)*(ncols+1),&values);CHKERRQ(ierr);
		ierr=PetscMalloc(sizeof(PetscScalar)*(ncols),&cols_p);CHKERRQ(ierr);
 		ierr=PetscMemcpy(values,vals,ncols);CHKERRQ(ierr);
		ierr=PetscMemcpy(cols_p,cols,ncols);CHKERRQ(ierr);
		//  invert them
		for(j=0;j<ncols;j++){
			values[j]=-values[j];
			cols_p+=sizecol;
		}
		// and set again with shift of sizerow
		ierr=MatSetValues(*r2c,1,&i,ncols,cols_p,vals,INSERT_VALUES);CHKERRQ(ierr);
		// set first imaginary values part first shift
		icol=i+sizerow;
		ierr=MatSetValues(*r2c,1,&icol,ncols,cols,vals,INSERT_VALUES);CHKERRQ(ierr);
		ierr=PetscFree(values);CHKERRQ(ierr);
		ierr=PetscFree(cols_p);CHKERRQ(ierr);

		ierr=MatAssemblyBegin(*r2c,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
    ierr=MatAssemblyEnd(*r2c,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
	}
	return ierr;
}

PetscErrorCode real2complexVec(Vec *in, Vec * out){
	PetscErrorCode ierr;
	PetscInt vsize,i;
	PetscScalar *varray;

	ierr=VecGetSize(*in,&vsize);CHKERRQ(ierr);
	ierr=VecCreate(PETSC_COMM_WORLD,out);CHKERRQ(ierr);
	ierr=VecSetSizes(*out,PETSC_DECIDE,vsize*vsize);CHKERRQ(ierr);
	ierr=VecGetArray(*in,&varray);

	for(i=0;i<vsize;i++){
		VecSetValue(*out,i,varray[i],INSERT_VALUES);
		VecSetValue(*out,i+vsize,-varray[i],INSERT_VALUES);
	}
	ierr=VecRestoreArray(*in,&varray);
	
	return ierr;
}
