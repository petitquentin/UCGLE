/*Copyright (c) 2011—2016. Pierre-Yves AQUILANTI and Xinzhe WU in Maison de la Simulation. All rights reserved */
#include "triep.h"

/* 
! -----------------------------------------------------------------------
! Classe les valeurs propres dans l'ordre croissant des parties reelles
! Repere le rang de la derniere ayant une partie reelle negative
! -----------------------------------------------------------------------
 */
PetscErrorCode tri(PetscScalar * vp, PetscInt nValues, PetscInt * ch_signe){
	PetscInt i;
	/* 
	! tri par rapport aux parties reelles
	 */
	 
	for( i = 0; i < nValues-1 ; ++i)
	{
		sort(vp,0,nValues);
		

	/* 
	 ! Comptage des valeurs propres ayant leur partie reelle negative
 	*/	
 
 		*ch_signe=0;
 	
		while(PetscRealPart(PetscRealPart(vp[*ch_signe]))<0.0){
			(*ch_signe)++;
			if(*ch_signe>nValues){
				break;
			}
		}
	}	
	
	
	return 0;
}

/* 
! Elimine les valeurs propres dont la partie imaginaire est negative
 */
PetscErrorCode epurer(PetscScalar * vp, PetscInt * nValues){
	PetscInt i,nKeep;
	PetscReal tmp;
	PetscScalar * eigen_keep;
	
	PetscMalloc((*nValues)*sizeof(PetscScalar),&eigen_keep);
	
	/* keep values with non negative imaginary part and non nul real part*/
	nKeep=0;
		
	for(i=0;i<*nValues;i++){
		/* abs() function does not appear as reliable, just a tweak in case of*/

///////////////

		if(PetscRealPart(vp[i])<0.0) tmp=-PetscRealPart(vp[i]);
		else tmp=PetscRealPart(vp[i]);

//////////////
		
/////		tmp=PetscRealPart(vp[i]);

		#ifdef DEBUG
		printf("$} Triep epure %e %e (%e)\n",PetscImaginaryPart(vp[i]),tmp,PetscRealPart(vp[i]));
		#endif
		if(PetscImaginaryPart(vp[i])>=0.0 && tmp!=0.0 && tmp>1.e-06){
			eigen_keep[nKeep]=vp[i];
			nKeep++;
		}		
	}
	
	for(i=0;i<nKeep;i++)
		vp[i]=eigen_keep[i];
	*nValues=nKeep;
	
	
	
	PetscFree(eigen_keep);
	
	return 0;
}

PetscErrorCode keepPositif(PetscScalar * vp, PetscInt * nValues){
  PetscInt i,nKeep;
  PetscReal tmp;
  PetscScalar * eigen_keep;

  PetscMalloc((*nValues)*sizeof(PetscScalar),&eigen_keep);

  /* keep values with non negative imaginary part and non nul real part*/
  nKeep=0;

  for(i=0;i<*nValues;i++){
    /* abs() function does not appear as reliable, just a tweak in case of*/

    ///////////////
    /*
    if(PetscRealPart(vp[i])<0.0) tmp=-PetscRealPart(vp[i]);
    else tmp=PetscRealPart(vp[i]);

    //////////////

    /////           tmp=PetscRealPart(vp[i]);

                #ifdef DEBUG
    printf("$} Triep epure %e %e (%e)\n",PetscImaginaryPart(vp[i]),tmp,PetscRealPart(vp[i]));
                #endif
    */

    if(PetscRealPart(vp[i])>0.0){
      eigen_keep[nKeep]=vp[i];
      nKeep++;
    }

  }

  for(i=0;i<nKeep;i++)
    vp[i]=eigen_keep[i];

  *nValues=nKeep;



  PetscFree(eigen_keep);

  return 0;
}

PetscErrorCode keepNegatif(PetscScalar * vp, PetscInt * nValues){
  PetscInt i,nKeep;
  PetscReal tmp;
  PetscScalar * eigen_keep;

  PetscMalloc((*nValues)*sizeof(PetscScalar),&eigen_keep);

  /* keep values with non negative imaginary part and non nul real part*/
  nKeep=0;

  for(i=0;i<*nValues;i++){
    /* abs() function does not appear as reliable, just a tweak in case of*/

    ///////////////
    /*
    if(PetscRealPart(vp[i])<0.0) tmp=-PetscRealPart(vp[i]);
    else tmp=PetscRealPart(vp[i]);

    //////////////

    /////           tmp=PetscRealPart(vp[i]);

                #ifdef DEBUG
    printf("$} Triep epure %e %e (%e)\n",PetscImaginaryPart(vp[i]),tmp,PetscRealPart(vp[i]));
                #endif
    */

    if(PetscRealPart(vp[i])<0.0){
      eigen_keep[nKeep]=vp[i];
      nKeep++;
    }

  }

  for(i=0;i<nKeep;i++)
    vp[i]=eigen_keep[i];

  *nValues=nKeep;



  PetscFree(eigen_keep);

  return 0;
}




void swap(PetscScalar *a, PetscScalar *b)
{
  PetscScalar t=*a; *a=*b; *b=t;
}


void sort(PetscScalar *arr, int beg, int end)
{
  if (end > beg + 1)
  {
    PetscScalar piv = arr[beg];
    int l ;
		int r;
    l=beg + 1;
     r = end;
    while (l < r)
    {
      if (PetscRealPart(arr[l]) <= PetscRealPart(piv))
        l++;
      else
        swap(&arr[l], &arr[--r]);
    }
    swap(&arr[--l], &arr[beg]);
    sort(arr, beg, l);
    sort(arr, r, end);
  }
}










