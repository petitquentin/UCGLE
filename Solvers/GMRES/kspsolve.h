/*

This file is part of software for the implementation of UCGLE method, under the supervision of Serge G. Petiton
<serge.petiton@univ-lille1.fr>.

Copyright (C) 2011—. Pierre-Yves AQUILANTI and Xinzhe WU <xinzhe.wu@ed.univ-lille1.fr> in Maison de la Simulation. 
All rights reserved.

Permission to use, copy, modify and distribute this software for personal and educational use is hereby granted
without fee, provided that the above copyright notice appears in all copies and that both that copyright notice 
and this permission notice appear in supporting documentation, and that the names of all authors are not used in
advertising or publicity pertaining to distribution of the software without specific, written prior permission. 
Addison Wesley Longman and the author make no representations about the suitability of this software for any 
purpose. It is provided "as is" without express or implied warranty.

You should have received a copy of the GNU Lesser General Public License along with UCGLE.  If not, see 
<http://www.gnu.org/licenses/>.

*/

#ifndef MY_KSP_SOLVE_H
#define MY_KSP_SOLVE_H
#include "petsc.h"
#include "../../Libs/mpi_lsa_com.h"
#include "gmres_solve.h"


PetscErrorCode MyKSPSolve(KSP ksp,Vec b,Vec x,com_lsa * com);


#endif