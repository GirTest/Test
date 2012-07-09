/*---------------------------------------------------------------------------*/
/* Project name		: PSP Training Program 4								 */
/* description		: Program to calculate relative size ranges for Very	 */
/*					  Small (VS), Small (S), Medium (M), Large (L) & Very	 */
/*					  Large (VL) ranges using standard deviation of an		 */
/*					  assumed log-normal distribution of sizes. The data set */
/*					  will be given for the different parts of a source code */
/*					  with number of parts or for the different sections of  */
/*					  a document. Data type is asked first to select between */ 
/*					  LOC & Page, If LOC, data is expected to be LOC on first*/ 
/*					  file of parts on second file with each part count		 */
/*					  seperated with a space. If Page, then only part size is*/
/*					  expected to seperated by white space. file name would  */
/*					  be DIP1.txt & DIP2.txt on project folder. base code for*/
/*					  the program is Program3								 */
/* Author Name		: Gireesh Shankarappa									 */
/* Created Date		: 14-Jun-2012											 */
/* Compile & Run Instructions	: Compile & build the project in Visual		 */
/*								  Studio C++ environment,create DataInput.txt*/
/*								  file, populate data seperated with white   */
/*								  spaces. Run in Debug mode & observe the	 */
/*							      results in terminal window				 */
/* Files Required				: None										 */
/* Modifications History													 */
/*	Modified date		:													 */
/*	Modified version	:    												 */
/*	Description			:													 */
/*	Modification Tag	:													 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Part Type : Pre_Processor												 */
/* Part Name : macro														 */
/*---------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>



/*---------------------------------------------------------------------------*/

#define FILE_X_NAME_STR	"DIP1.txt"	
#define FILE_Y_NAME_STR	"DIP2.txt"	
#define FILE_OPEN_MODE	"r"


typedef struct ST_NODE
{
	float fPartSize;
	float fNumOfParts;
	float fSizePerPart;
	float fSppLog;
	struct ST_NODE *p_stLink;
}ST_NODE;

/*---------------------------------------------------------------------------*/
/* End of Part Name : macro													 */
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/* Main Function */
/*---------------------------------------------------------------------------*/
int main ( void )
{
	FILE			*FileXPtr;
	FILE			*FileYPtr;
	ST_NODE			*p_stHeadPt;
	ST_NODE			*p_stTmp;	
	ST_NODE			*p_stTmp1;
	float			fDataRd;
	unsigned int	unDatalen = 0;

	float  			fAvgSppLog = 0;
	float  			fStdDev = 0;
	float  			fVSrange = 0;
	float			fSrange = 0;
	float			fMrange = 0;
	float			fLrange = 0;
	float		 	fVLrange = 0;
	char			cChar = 0;


/*---------------------------------------------------------------------------*/
/* Part Type : Input														 */
/* Part Name : File_Read													 */
/*---------------------------------------------------------------------------*/

	/* Check data type LOC/ Pages */
	printf("\nPlease select data type LOC or Pages.");
	printf("\nPress 'l' to select LOC or press 'p' to select Pages");
	fflush(stdin);
	scanf("%c", &cChar);


	/* Open the data X input file in read mode which will be placed in project folder */
	FileXPtr = fopen( FILE_X_NAME_STR, FILE_OPEN_MODE );
	if( FileXPtr == NULL )
	{
		printf( "\n Cannot open the file" );
		exit(1);
	}

	if(cChar == 'l')
	{
		/* Open the data Y input file in read mode which will be placed in project folder */
		FileYPtr = fopen( FILE_Y_NAME_STR, FILE_OPEN_MODE );
		if( FileYPtr == NULL )
		{
			printf( "\n Cannot open the file" );
			exit(1);
		}
	}

/*---------------------------------------------------------------------------*/
/* End of Part Name : File_Read												*/
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* Part Type : Process														 */
/* Part Name : List															 */
/*---------------------------------------------------------------------------*/

	/* Read the first set of data elements, Create the list & Populate dataX */
	while( EOF != fscanf( FileXPtr, "%f", &fDataRd ) )
	{
		/* Create a new node */
		p_stTmp = ( ST_NODE* ) malloc( sizeof( ST_NODE ) );
		p_stTmp->fPartSize = fDataRd;

		if(cChar == 'l')
		{
			fscanf( FileYPtr, "%f", &fDataRd );
			p_stTmp->fNumOfParts = fDataRd;
		}
		else
		{
			p_stTmp->fNumOfParts = 1;
		}

		p_stTmp->fSizePerPart = (p_stTmp->fPartSize / p_stTmp->fNumOfParts);
		p_stTmp->fSppLog = (float)log(p_stTmp->fSizePerPart);


		p_stTmp->p_stLink = NULL;
		if( unDatalen == 0 )
		{
			/* Have the header backup */
			p_stHeadPt = p_stTmp;	
		}
		else
		{
			/* Link to the previous node */
			p_stTmp1->p_stLink = p_stTmp;
		}
		p_stTmp1 = p_stTmp;
		
		unDatalen++;

		/* Display what is populated for data X */
		printf( "\n %d	: \t %3.2f , \t %3.2f , \t %3.2f , \t %3.2f", 
			    unDatalen, p_stTmp->fPartSize, p_stTmp->fNumOfParts, 
				p_stTmp->fSizePerPart, p_stTmp->fSppLog );
	}

	fclose( FileXPtr );
	if(cChar == 'l')
	{
		fclose( FileYPtr );
	}

/*---------------------------------------------------------------------------*/
/* End of Part Name : List													 */
/*---------------------------------------------------------------------------*/




/*---------------------------------------------------------------------------*/
/* Part Type : Calculation													 */
/* Part Name : parameters														 */
/*---------------------------------------------------------------------------*/
	/* Calculate Average */
	p_stTmp = p_stHeadPt;
	do
	{
		fAvgSppLog += p_stTmp->fSppLog;
		p_stTmp = p_stTmp->p_stLink;
	}while( p_stTmp != NULL );

	fAvgSppLog = (float)(fAvgSppLog / unDatalen);

	/* Calculate standard deviation */
	p_stTmp = p_stHeadPt;
	do
	{
		fStdDev += ( (p_stTmp->fSppLog - fAvgSppLog) * (p_stTmp->fSppLog - fAvgSppLog) );
		p_stTmp = p_stTmp->p_stLink;
	}while( p_stTmp != NULL );

	fStdDev = (float)(fStdDev / (unDatalen - 1));
	fStdDev = (float)sqrt(fStdDev);




	/* Calculate logorithmic ranges */
	fVSrange = (float)exp( fAvgSppLog - (2 * fStdDev ) );
	fSrange = (float)exp( fAvgSppLog - fStdDev );
	fMrange = (float)exp( fAvgSppLog );
	fLrange = (float)exp( fAvgSppLog + fStdDev );
	fVLrange = (float)exp( fAvgSppLog + (2 * fStdDev ) );

	/*---------------------------------------------------------------------------*/
/* End of Part Name : parameters													 */
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/* Part Type : Output														 */
/* Part Name : Display														 */
/*---------------------------------------------------------------------------*/

	/* Display length of data */
	printf( "\nNumber of data elements = %d\n", unDatalen );

	/* Display the results */
	printf( "\n Very Small		= %4.4f", fVSrange );
	printf( "\n Small			= %4.4f", fSrange );
	printf( "\n Medium			= %4.4f", fMrange );
	printf( "\n large			= %4.4f", fLrange );
	printf( "\n Very large		= %4.4f", fVLrange );
	printf( "\n -------------------------------------- \n" );

/*---------------------------------------------------------------------------*/
/* End of Part Name : Display													 */
/*---------------------------------------------------------------------------*/


	/* Should fre the memory aquired for linked list */
	/* Should sleep for about 30seconds or 1 minute till the user looks into 
	the output or wiat indefinitely */

	/* Wait indefinitely till program is terminatde manually */
	while(1);

}
/*---------------------------------------------------------------------------*/



/* Dummy line added to evaluate GitHub */