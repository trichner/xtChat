// C Source File
// Created 21.01.2009; 17:37:31





#define COMMENT_PROGRAM_NAME   "Key Interrupt Test"
#define COMMENT_VERSION_STRING "Vers 1.0"

#define SAVE_SCREEN
#define USE_V200              // Compile for V200


#define EIN_X 3
#define EIN_Y 110
#define LOG_X 3
#define LOG_Y 122

#define MAXLEN 40


#include <tigcclib.h>


INT_HANDLER oldint4 = NULL;

//========================Variabeln===========================

	unsigned char err; 			//Anzahl empfangene Bytes pro Abfrage
	unsigned char buc; 			//empfangenes Byte
	unsigned char txt[50];	//EmpfangsBuffer
	unsigned char a=0;			//y-Richtung write
	unsigned char b=0;			//Anzahl Interruptaufrufe
	unsigned char x=0;			//x-Richtung write
	unsigned char n=1;			//Anzahl empfangene Bytes
	unsigned short key = 32;
	int ring=0;
  volatile BOOL link_active=FALSE;
  volatile BOOL lcd_change=TRUE;
  
  unsigned char zeile[10][40];
  unsigned char *history[10]; 

//==============================================Interrupt Routine===============================================================
DEFINE_INT_HANDLER (LPA) {

    link_active=TRUE;
    ExecuteHandler(oldint4);
}



//=================================================== Function ==================================================================

void hist(unsigned char *arr)
{
 int z;
 for(z=9;z>=1;z--)
 {
 	history[z]=history[z-1]; 	
 	//history[z]++;
 }

 strcpy(zeile[ring],arr);
 history[0]=&zeile[ring];   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! -> Assignment from incompatible pointer type. !!!!!!!!!!!!!!!!

 
 if(ring<9)
 	ring++;
 else
 	ring = 0;

};



	  	
	  


//============================================== Main Function ==================================================================

void _main(void)
{
  //initialisieren der Variabeln & Link
  //printf_xy(10,10,"Program Initialized");
	ClrScr();
	//printf_xy(10,10,"Screen Cleared");
	GKeyFlush();
	//printf_xy(10,10,"GKeyFlush");
  
  
  
  OSLinkOpen();
  
  FontSetSys(F_4x6);
  printf_xy(LOG_X,LOG_Y,"Link opened");
  FontSetSys(F_6x8);
  
  //Interrupt Vector setzen
  oldint4 = GetIntVec(AUTO_INT_4);
  SetIntVec(AUTO_INT_4, LPA); 		 //Key Pressed
  
  //printf_xy(10,10,"                                  ");

	/*
	FontSetSys(F_4x6);
  printf_xy(10,10,"Font Set");
  */
  


  
  //unsigned char 
  
  unsigned char retxt[40];
  unsigned char buffer[40];
  unsigned char i;
  unsigned char z;
  unsigned char h;
  int zael;
    
  i=0;
  n=1;
  h=0;
  ring=0;
  
  
	
	for(z=0;z<=10;z++)
	{
	  //history[z]=history[z-1]
	  for(zael=0;zael<41;zael++)
	  {
	  	zeile[z][zael]=' ';	
	  	buffer[zael]=0;
			retxt[zael]=0;
	  }
		
		history[z]=&zeile[z];
	}
	
	printf_xy(EIN_X,EIN_Y,"_  ");
  
  FontSetSys(F_4x6);
  printf_xy(LOG_X,LOG_Y,"init hist...          ");
  
  hist(zeile[2]);

  printf_xy(LOG_X,LOG_Y,"started...             ");
  FontSetSys(F_6x8);
	while(n==1)
	{
	
	if(lcd_change)
	{
	  	FontSetSys(F_4x6);
			printf_xy(LOG_X,LOG_Y,"lcd_change...                ");
	    printf_xy(LOG_X,LOG_Y,"hist ok               ");
	    FontSetSys(F_6x8);
			DrawStr(3, 85,("                                          "),A_REPLACE);
			DrawStr(3, 77,("                                          "),A_REPLACE);
			DrawStr(3, 69,("                                          "),A_REPLACE);
			DrawStr(3, 61,("                                          "),A_REPLACE);
			DrawStr(3, 53,("                                          "),A_REPLACE);
			DrawStr(3, 45,("                                          "),A_REPLACE);
			DrawStr(3, 37,("                                          "),A_REPLACE);
			DrawStr(3, 29,("                                          "),A_REPLACE);
			DrawStr(3, 21,("                                          "),A_REPLACE);
			DrawStr(3, 13,("                                          "),A_REPLACE);
	
	    	    
			DrawStr(3, 85,history[0] ,A_REPLACE);
			DrawStr(3, 77,history[1],A_REPLACE);
			DrawStr(3, 69,history[2],A_REPLACE);
			DrawStr(3, 61,history[3],A_REPLACE);
			DrawStr(3, 53,history[4],A_REPLACE);
			DrawStr(3, 45,history[5],A_REPLACE);
			DrawStr(3, 37,history[6],A_REPLACE);
			DrawStr(3, 29,history[7],A_REPLACE);
			DrawStr(3, 21,history[8],A_REPLACE);
			DrawStr(3, 13,history[9],A_REPLACE);
			
			lcd_change=FALSE;
	}
	FontSetSys(F_4x6);
	printf_xy(LOG_X,LOG_Y,"LPM...              ");
		pokeIO(0x600005,0b11111);   //LPM
  //printf_xy(LOG_X,LOG_Y,"exit LPM              ");
	FontSetSys(F_6x8);
  
  //------------------------------------ Linkport aktiv
  if (link_active) 
  {
    FontSetSys(F_4x6);
    printf_xy(LOG_X,LOG_Y,"link active...             "); 	
    FontSetSys(F_6x8);  
	  err=LIO_RecvData(&retxt,40,5);
		link_active = FALSE;
		
	  if(!err)
	  {
	  	//strcpy(retxt,"hallo welt das war nicht gut");
	  	//DrawStr(10,50,("                                           "),A_REPLACE); //eingabe Löschen
 	 		//printf_xy(10,50,"%s  ",retxt);
 	 		
 	 		hist(retxt);
 	 		
 	 		lcd_change=TRUE;

 	 		
	  }
	  
	        
  }//-----------------------------------  AI4


  //------------------------------------ Tastendruck
  if (kbhit()) 
  {
      FontSetSys(F_4x6);
      printf_xy(LOG_X,LOG_Y,"kbhit...           ");
      FontSetSys(F_6x8);
      key = ngetchx ();
     
      if (key >= ' ' && key <= '~' && i < MAXLEN)
        buffer[i++] = key;
      else if (key == KEY_BACKSPACE && i)
        i--;
      else if (key == KEY_ENTER)
      {
      	//---Text Löschen---
      	DrawStr(EIN_X,EIN_Y,("_                                         "),A_REPLACE); //eingabe Löschen
				
				
      	err = LIO_SendData(&buffer, 40); //SENDEN
  		    		 
  			if(err)		
				{		
				  FontSetSys(F_4x6);		
				  printf_xy(LOG_X,LOG_Y,"Send Error           ");
				  FontSetSys(F_6x8);
				}   
      	
				
				hist(buffer);
				
				for(z=0;z<41;z++)
				{
					buffer[z]=0;
				}
				
				i=0;
				lcd_change=TRUE;
				
      }else if(key == KEY_ESC)
      {
      	n=0;
      }
       
      printf_xy(EIN_X,EIN_Y,"%s_  ", buffer);
      buffer[i] = 0;
      


  }//----------------------------------- AI2
	
	
	} //main-schleife



  //Reset Interrupt
  SetIntVec(AUTO_INT_4, oldint4);
  
  //Beenden
  GKeyFlush();
  
  OSLinkClose();

}
