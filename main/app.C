#include "functions.h"
int main(int argc,char *argv[]) {
    if(argc<2){
            //system_clear
        system_clear();

        
        

        //pointer call
        setPointer();
        int pointer = getPointer();
        increasePointer();
        
        //welcome log    
        logWelcome(1);
        
        //parser
        while(1){
        printf("\n>>> ");
        char command[length];
        fgets(command,sizeof(command),stdin);
        if((strncmp(command,"CLR",3)==0) || (strncmp(command,"REFRESH",7)==0)){
            #ifdef _WIN32
                //for windows
                system("cls");
                logWelcome(0);
            #else
                //for mac and Linux based os(distros)
                system("clear");
            #endif
        }else if(strncmp(command,"EXIT",4)==0){
        break;
        }else if(strncmp(command,"USE",3)==0){
            logUse();
        }else if(strncmp(command,"DATA",4)==0){
            logData();
        }else if(strncmp(command,"LOG",3)==0){
            LOG();
        }else if(strncmp(command,"HELP",4)==0){
            HELP();
        }
        else{
            
            execute(command);
        }
        }
    }else{
        //for sample API of feed file command
        if(argc>1){
            if(strncmp(argv[1],"feed",4)==0 && strncmp(argv[2],"file",4)==0){
                call_feed_file(argv[3],argv[4]);
            }
        }
    }
    



   return 0;
   
}









