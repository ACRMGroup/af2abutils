/************************************************************************/
/**

   Program:    pdbrmseq
   \file       pdbrmseq.c
   
   \version    V1.0
   \date       10.09.21   
   \brief      Removes the first occurrence of a specified sequence
               from a PDB file
   
   \copyright  (c) UCL / Prof. Andrew C. R. Martin 2021
   \author     Prof. Andrew C. R. Martin
   \par
               Institute of Structural & Molecular Biology,
               University College,
               Gower Street,
               London.
               WC1E 6BT.
   \par
               andrew@bioinf.org.uk
               andrew.martin@ucl.ac.uk
               
**************************************************************************

   This program is not in the public domain, but it may be copied
   according to the conditions laid out in the accompanying file
   COPYING.DOC

   The code may be modified as required, but any modifications must be
   documented so that the person responsible can be identified.

   The code may not be sold commercially or included as part of a 
   commercial product except as described in the file COPYING.DOC.

**************************************************************************

   Description:
   ============

**************************************************************************

   Usage:
   ======

**************************************************************************

   Revision History:
   =================

*************************************************************************/
/* Includes
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bioplib/general.h"
#include "bioplib/pdb.h"
#include "bioplib/seq.h"
#include "bioplib/macros.h"

/************************************************************************/
/* Defines and macros
*/
#define MAXBUFF 240

/************************************************************************/
/* Globals
*/

/************************************************************************/
/* Prototypes
*/
int  main(int argc, char **argv);
BOOL ParseCmdLine(int argc, char **argv, char *infile, char *outfile,
                  char *sequence);
void Usage(void);
PDB *RemoveSequence(PDB *pdb, char *sequence);


/************************************************************************/
/*>int main(int argc, char **argv)
   -------------------------------
*//**

   Main program for removing a specified sequence from a PDB file

-  10.09.21 Original    By: ACRM
*/
int main(int argc, char **argv)
{
   FILE *in  = stdin,
        *out = stdout;
   char infile[MAXBUFF],
        outfile[MAXBUFF],
        sequence[MAXBUFF];
   PDB  *pdb;
   int  natom;
        
   if(ParseCmdLine(argc, argv, infile, outfile, sequence))
   {
      if(blOpenStdFiles(infile, outfile, &in, &out))
      {
         if((pdb=blReadPDB(in, &natom))==NULL)
         {
            fprintf(stderr,"No atoms read from input file\n");
         }
         else
         {
            PDB *pdbOut = RemoveSequence(pdb, sequence);
            blWritePDB(out, pdbOut);
         }
      }
      else
      {
         Usage();
         return(1);
      }
   }
   else
   {
      Usage();
      return(1);
   }
   
   return(0);
}


/************************************************************************/
/*>BOOL ParseCmdLine(int argc, char **argv, char *infile, char *outfile,
                     char *sequence)
   ----------------------------------------------------------------------
*//**

   \param[in]      argc        Argument count
   \param[in]      **argv      Argument array
   \param[out]     *infile     Input filename (or blank string)
   \param[out]     *outfile    Output filename (or blank string)
   \param[out]     *sequence   Sequence to remove
   \return                     Success

   Parse the command line

-  10.09.21 Original    By: ACRM
*/
BOOL ParseCmdLine(int argc, char **argv, char *infile, char *outfile,
                  char *sequence)
{
   argc--;
   argv++;
   
   infile[0] = outfile[0] = '\0';
   
   while(argc>0)
   {
      if(argv[0][0] == '-')
      {
         switch(argv[0][1])
         {
         case 'h':
            return(FALSE);
            break;
         default:
            return(FALSE);
            break;
         }
      }
      else
      {
         /* Check that there are 1-3 arguments left                     */
         if((argc < 1) || (argc > 3))
            return(FALSE);
         
         /* Copy the first to sequence                                  */
         strcpy(sequence, argv[0]);
         
         /* If there's another, copy it to infile                       */
         argc--;
         argv++;
         if(argc>0)
            strcpy(infile, argv[0]);

         /* If there's another, copy it to outfile                      */
         argc--;
         argv++;
         if(argc>0)
            strcpy(outfile, argv[0]);

         return(TRUE);
      }
      argc--;
      argv++;
   }
   
   return(TRUE);
}


/************************************************************************/
PDB *RemoveSequence(PDB *pdb, char *sequence)
{
   char *pdbSeq;
   PDB  *pdbOut = NULL;

   if((pdbSeq = PDBProt2SeqX(pdb))!=NULL)
   {
      char *seqStart = strstr(pdbSeq, sequence);
      if(seqStart)
      {
         PDB *p       = NULL,
             *q       = NULL,
             *r       = NULL,
             *nextRes = NULL;
         int offset   = (int)(seqStart - pdbSeq);
         int seqLen   = strlen(sequence);
         int count    = 0;
         UPPER(sequence);

         for(p=pdb; p!=NULL; p=nextRes)
         {
            nextRes = blFindNextResidue(p);
            if((count < offset) || (count >= offset+seqLen))
            {
               for(q=p; q!=nextRes; NEXT(q))
               {
                  if(pdbOut == NULL)
                  {
                     INIT(pdbOut, PDB);
                     r=pdbOut;
                  }
                  else
                  {
                     ALLOCNEXT(r, PDB);
                  }

                  if(r==NULL)
                  {
                     fprintf(stderr, "Error: no memory for new PDB list");
                     exit(1);
                  }
                  blCopyPDB(r,q);
               }
            }
            
            count++;
         }
      }
      else
      {
         fprintf(stderr, "Warning: sequence not found\n");
         pdbOut = pdb;
      }
   }
   else
   {
      fprintf(stderr, "Error: no memory to extract sequence\n");
   }

   return(pdbOut);
}

void Usage(void)
{

}
