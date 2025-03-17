/*Written as a C project. This file provides a basic functionality for easy file manipulation...I made it personally to be like mongodb with sql like easy command. But not succeeded. However provided basic file mainpulation locally on system. Functions can be used and be adjusted as per the main file and execute() executer in this file..*/

#ifndef FUNCTIONS_H
#define FUNCTIONS_H




//return values
#define isExist 1
#define notExist -1
#define errorCreatingFile 404
#define success 10
#define unknownError 100
#define exiting 11

//mem-declarations
#define length 256

//essential
#include <stdio.h>
#include <string.h>
#include <stdlib.h>//for system("cls")
#include <unistd.h>///for sleep()


//clear_system
void system_clear(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}



//location
static char dir[]="D:";

//folders
static const char DETAILS_folder[length]="D:/Database in C with API/DETAILS";
static const char data_folder[length]="D:/Database in C with API/data";
static const char internals_folder[length]="D:/Database in C with API/internals";
static const char VERSION_folder[length]="D:/Database in C with API/VERSION";

//files
static const char help_file[length]="D:/Database in C with API/DETAILS/help";
static const char log_file[length]="D:/Database in C with API/DETAILS/log";
static const char contentVar_file[length]="D:/Database in C with API/internals/contentVar";
static const char files_file[length]="D:/Database in C with API/internals/files";
static const char pointer_file[length]="D:/Database in C with API/internals/pointer";
static const char version_file[length]="D:/Database in C with API/internals/version";


//for use-variable
struct value {
    int id;
    char path[length];
};



//pointer
static int pointer;
void setPointer(void){
    FILE *fp = fopen(pointer_file,"r");
    if(fp==NULL){
        printf("No file pointer found");
    }else{
        fscanf(fp,"%d",&pointer);
        fclose(fp);
    }
}

//increase pointer
void increasePointer(void){
    pointer = pointer +1;
    FILE *fp = fopen(pointer_file,"w");
    fprintf(fp,"%d",pointer);
    fclose(fp);
}

//get-pointer
int getPointer(){
    return pointer;
}



//checkFile
int checkFile(const char filename[],int id){
    int tempid;
    char tempname[length];
    char location[length];
    
    strcpy(location,files_file);

    FILE *fp = fopen(location,"r");

    while(fscanf(fp,"name=%s id=%d\n",tempname,&tempid)==2){
        if(strcmp(tempname,filename)==0){
        //    return isExist;
            if(id==tempid){
                fclose(fp);
                return isExist;
            }
        }
    }
    return notExist;
    
}

//getSize
int getSize(const char filename[],int id){
    
    char path[length];
    snprintf(path,sizeof(path),"%s/%s",data_folder,filename);

    int check = checkFile(filename,id);
    if(check == isExist){
        FILE *fp = fopen(path,"r");
        fseek(fp,0,SEEK_END);
        int size_byte = ftell(fp);
        rewind(fp);
        fclose(fp);
        return size_byte;
    }else if(check == notExist){
        return notExist;
    }else{
        return unknownError;
    }
}

//useVariable

static char usePath[length];
static int useId;
static char useName[length];
static long int useSize;
int use(char filename[],int id){
    int value = checkFile(filename,id);
    if(value==isExist){
        struct value use;

        char filePath[length];
        
        snprintf(filePath,sizeof(filePath),"%s/%s",data_folder,filename);

        use.id= id;
        useId=use.id;
        useSize= getSize(filename,id);
        strcpy(useName,filename);
        strcpy(use.path,filePath);
        strcpy(usePath,use.path);
        return success;
    }else if (value == notExist){
        return notExist;
    }else{
        return unknownError;
    }
    
}


//sushil start
//createFile
int createFile(const char filename[],int id){
    if(checkFile(filename,id)==notExist){
        char filePath[length];
        
        snprintf(filePath,sizeof(filePath),"%s/%s",data_folder,filename);
        FILE *fp = fopen(filePath,"w");
        if(fp==NULL){
            return errorCreatingFile;
        }else{
            FILE *files = fopen(files_file,"a");
            if(files ==NULL){
                perror("Error:");
            }
            fprintf(files,"name=%s id=%d\n",filename,id);
            fclose(files);
            fclose(fp);    
            return success;
        }
        
    }else{
        return isExist;
    }
}

//deleteFile
int deleteFile(const char filename[],int id){
    if(checkFile(filename,id)==isExist){
        char filePath[length];
        snprintf(filePath,sizeof(filePath),"%s/%s",data_folder,filename);
        remove(filePath);
        
        //set the file to delete
        FILE *f = fopen(files_file,"r");
        
        char tempname[length];
        int tempid;

        //storing in garbage file
        char tempPath[]="garbage";
        FILE *temp = fopen(tempPath,"w");
        while(fscanf(f,"name=%s id=%d\n",tempname,&tempid)==2){
            if(strcmp(tempname,filename)==0 && id==tempid){
                continue;
            }else{
                fprintf(temp,"name=%s id=%d\n",tempname,tempid);
            }
        }
        fclose(f);
        fclose(temp);

        //restoring original
        remove(files_file);

        FILE *files = fopen(files_file,"w");
        FILE *tempNew = fopen(tempPath,"r");
        while(fscanf(tempNew,"name=%s id=%d\n",tempname,&tempid)==2){
            fprintf(files,"name=%s id=%d\n",tempname,tempid);
        }
        
        fclose(files);
        fclose(tempNew);
        remove(tempPath);

        return success;

    }else{
        return notExist;
    }
}


//feedFile
int feedFile(char filename[],int id){
    int tempExist = checkFile(filename,id);
    if(tempExist== isExist){
        char input[length];
        printf("Type data to enter....\n");
        while(1){
            fgets(input,sizeof(input),stdin);

            if(strncmp(input,"..exitFeed",10)==0){
                return exiting;
            }else{
                char filePath[length];
                
                snprintf(filePath,sizeof(filePath),"%s/%s",data_folder,filename);
                FILE *fp= fopen(filePath,"a");
                if(fp!=NULL){
                    fprintf(fp,"%s",input);
                    fclose(fp);  
                }else{
                    return unknownError;
                }
            }
        }
    }else{
        return notExist;
    }
}


//flushFile
int flushFile(const char filename[],int id){
    int value = checkFile(filename,id);
    if(value==isExist){
        char filePath[length];
        
        snprintf(filePath,sizeof(filePath),"%s/%s",data_folder,filename);
        remove(filePath);
        FILE * fp = fopen(filePath,"w");
        if(fp==NULL){
            return unknownError;
        }else{
            return success;
        }
        
    }else if(value == notExist){
        return notExist;
    }
}


//checkExist
int checkExist(char filename[],int id){
    int value = checkFile(filename,id);
    if(value==notExist){
        return notExist;
    }
    if(value == isExist){
        return isExist;
    }
}

///renderFile
int renderFile(char filename[],int id){
    int value = checkFile(filename,id);
    if(value == isExist){
        char filePath[length];
        
        snprintf(filePath,sizeof(filePath),"%s/%s",data_folder,filename);

        FILE *fp = fopen(filePath,"r");
        fseek(fp,0,SEEK_END);
        int position = ftell(fp);
        rewind(fp);

        if(position!=0){
        
            
        printf("-----------content from id:%d file:%s--------------\n",id,filename);
        printf("\n");
        
        char ch;
        while((ch=fgetc(fp))!=EOF){
            printf("%c",ch);
        }


        printf("\n");
        printf("------------------------------------------------------\n");
        fclose(fp);
        }else if(position==0){
            printf("-----------content from id:%d file:%s--------------\n",id,filename);
            printf("\n");
            
            
    
            printf("\n");
            printf("------------------------------------------------------\n");
            
            fclose(fp);
        }else{
            printf("unknown error\n");
        }

        
    }else if(value==notExist){
        printf("id:%d file:%s not exist\n",id,filename);
        return notExist;
    }else{
        printf("Unknown Error detected\n");
        return unknownError;
    }
}


//output size
int renderSize(char filename[],int id){
    int value = checkFile(filename,id);
    if(value == isExist){
        
        char filePath[length];
        snprintf(filePath,sizeof(filePath),"%s/%s",data_folder,filename);

        FILE *fp = fopen(filePath,"r");
        if(fp!=NULL){
            fseek(fp,0,SEEK_END);
            long size = ftell(fp);
            rewind(fp);
            fclose(fp);
            
            return size;
        }
    }else{
        return notExist;
    }
}








//replicate by asking
int askReplicate(const char filename[],int id){
  int value = checkFile(filename,id);
  if(value == isExist){

    printf("Enter id for new file: ");
    int tempid;
    scanf("%d",&tempid);

    printf("Enter filename for new file:");
    char tempname[length];
    scanf("%s",tempname);
    
    

    char filePathold[length];
    snprintf(filePathold,sizeof(filePathold),"%s/%s",data_folder,filename);

    char filePathnew[length];
    snprintf(filePathnew,sizeof(filePathnew),"%s/%s",data_folder,tempname);

    FILE *old = fopen(filePathold,"r");
    FILE *newFile  = fopen(filePathnew,"w");
    
    // char ch;
    // while((ch=fgetc(old))!=EOF){
    //     fputc(ch,newFile);
    // }


    fseek(old,0,SEEK_END);
    long int position = ftell(old);
    rewind(old);

    char * buffer = (char *)malloc(position+1);
    if(buffer!=NULL){
        
    fread(buffer,1,position,old);
    buffer[position]='\0';

    fprintf(newFile,"%s",buffer);

        
    //filelogger
    
    char path[length];
    snprintf(path,sizeof(path),"%s/%s",internals_folder,"files");
    FILE * loggerFile = fopen(path,"a");
    if(loggerFile==NULL){
        //debug
        printf("No logger file line ");
    }
    fprintf(loggerFile,"name=%s id=%d\n",tempname,tempid);

    fclose(loggerFile);
    fclose(newFile);
    fclose(old);

    printf("replicated id:%d file:%s as id:%d file:%s\n",id,filename,tempid,tempname);
    return success;
    }else{
        fclose(old);
        fclose(newFile);
        printf("Unknown Error occured\n");
        return unknownError;
    }

    
  }else {
    printf("id:%d file:%s not exist",id,filename);
    return notExist;
  }

}




//copy file into content-var
int copyFile(char filename[],int id){
    int check= checkFile(filename,id);
    if(check == isExist){
        
        char path[length];
        snprintf(path,sizeof(path),"%s/%s",data_folder,filename);

        FILE *fp =  fopen(path,"r");
        char ch;
        FILE *temp = fopen(contentVar_file,"w");
        while((ch=fgetc(fp))!=EOF){

            fprintf(temp,"%c",ch);
        }
            


            fclose(fp);
            fclose(temp);
            
            return success;
        
        
    }else{
        return notExist;
    }
}


//paste file from contentVar
int pasteFile(char filename[],int id){
    
    
    int check = checkFile(filename,id);
    if(check == isExist){
    FILE *sourceFile = fopen(contentVar_file,"r");
    
    
    char destPath[length];
    snprintf(destPath,sizeof(destPath),"%s/%s",data_folder,filename);

    FILE *destFile = fopen(destPath,"a");
    if(destFile!=NULL){

        char ch;
        while((ch=fgetc(sourceFile))!=EOF){
            fputc(ch,destFile);
        }
        fclose(sourceFile);
        fclose(destFile);
        return success;
        }else{
            
            return unknownError;
        }
    }
    else{
        return notExist;
    }

}


//prepare object
int prepareObject(char filename[],int id){
    int check = checkFile(filename,id);
    if(check==isExist){

        
        char path[length];
        snprintf(path,sizeof(path),"%s/%s",data_folder,filename);

        FILE *fp =  fopen(path,"a");

        struct object {
            char title[length];
        };
        struct value {
            char value[length];
        };
    
        int n=0;
        printf("Enter how many titles\n");
        scanf("%d",&n);
        struct object obj[n];
        struct value val[n];
    
        printf("Enter object title\n");
        for(register int i=0;i<n;i++){
            printf("\n%d.",i+1);
            scanf("%s",obj[i].title);
        }
        printf("enter number of objects \n");
        int a=0;
        scanf("%d",&a);
        for(int i=0;i<a;i++){
            printf("Enter value for %d object\n",i+1);
            for(int j=0;j<n;j++){
                printf("%s=",obj[j].title);
                scanf("%s",val[j].value);
                fprintf(fp,"%s=%s ",obj[j].title,val[j].value); 
                   
            }
            fprintf(fp,"\n");
            
            
        }

        printf("feeding done\n");
        fclose(fp);
        return success;
    
    }else{
        printf("id:%d file:%s not exist\n",id,filename);
        return notExist;
    }
        
}

//import 
int import_as(char filename[],int id){
    int value = checkFile(filename,id);
    if(value == isExist){
        return isExist;
    }else{
        printf("Specify path: \n");
        char path[length];
        scanf("%s",path);
        

        FILE *fp = fopen(path,"r");
        if(fp==NULL){
            return notExist;
        }else{
            char newPath[length];
            
            snprintf(newPath,sizeof(newPath),"%s/%s",data_folder,filename);
            
            FILE *file = fopen(newPath,"w");
            if(file == NULL){
                fclose(fp);
                
                return unknownError;
            }else{
                char ch;
                while((ch=fgetc(fp))!=EOF){
                    fputc(ch,file);
                }
                
                FILE *logger = fopen(files_file,"a");
                if(logger!=NULL){
                    fprintf(logger,"name=%s id=%d\n",filename,id);
                    fclose(logger);
                }
                fclose(fp);
                fclose(file);
                return success;

            }

        }
    }
}


int inject_into(char filename[],int id){
    int value = checkFile(filename,id);
    if(value == notExist){
        return notExist;
    }else{
        char path[length];
        snprintf(path,sizeof(path),"%s/%s",data_folder,filename);
        FILE *target= fopen(path,"a");
        if(target==NULL){
            printf("Error\n");
            return unknownError;
        }

        printf("Enter path:\n");
        
        char location[length];
        scanf("%s",location);
        FILE *source= fopen(location,"r");
        
        if(source==NULL){
            fclose(target);
            return notExist;
        }else{
            char ch;
            while((ch=fgetc(source))!=EOF){
                
                fputc(ch,target);
            }
            fclose(source);
            fclose(target);
            return success;
        }
    }
}



//execute
int execute(const char code[]){
    char command[length],method[length],name[length];
    int id;
    char tempUse[3];

    if(sscanf(code,"%s %s %s %d",command,method,name,&id)==4){
        if(strcmp(command,"create")==0){
            if(strcmp(method,"file")==0){
                //createFile
                int temp=createFile(name,id);
                if(temp==success){
                    
                    printf("id:%d & file:%s successfully created\n",id,name);
                }else if(temp==errorCreatingFile){
                    printf("Error occured");
                }else if(temp==isExist){
                    printf("id:%d & file:%s already exists\n",id,name);
                }else{
                    printf("Unknown error");
                }
            }
        }else if(strcmp(command,"delete")==0){
            if(strcmp(method,"file")==0){
                //delete
                int value = deleteFile(name,id);
                if(value==success){
                    printf("id:%d file:%s deleted\n",id,name);
                }else if(value == notExist){
                    printf("id:%d file:%s not exist\n",id,name);
                }
            }
        }else if(strcmp(command,"use")==0){
            if(strcmp(method,"file")==0){
                int value =use(name,id);
                if(value == success){
                    printf("use variable set to id:%d & file:%s\n",id,name);
                }else if(value == notExist){
                    printf("id:%d file:%s not exist\n",id,name);;
                }else{
                    printf("Error with usevariable\n");
                }
            }
        }else if(strcmp(command,"feed")==0){
            if(strcmp(method,"file")==0){
                int value= feedFile(name,id);
                if(value==notExist){
                    printf("id:%d file:%s not exist\n",id,name);
                }
                if(value == unknownError){
                    printf("Unknown Error detected\n");
                }
                if(value == exiting){
                    printf("Exit id:%d file:%s\n",id,name);
                }
            }else if(strcmp(method,"object")==0){
                //feed object
                prepareObject(name,id);
            }
        }else if(strcmp(command,"flush")==0){
            if(strcmp(method,"file")==0){
                int value = flushFile(name,id);
                if(value==success){
                    printf("id:%d file:%s flushed to new\n",id,name);
                }else if(value == notExist){
                    printf("id:%d file:%s not exist\n",id,name);
                }else{
                    printf("unknown Error detected\n");
                }
            }
        }else if(strcmp(command,"is")==0){
            if(strcmp(method,"exist")==0){
                int value = checkExist(name,id);
                if(value==isExist){
                    printf("Exist id:%d file:%s\n",id,name);
                }
                if(value == notExist){
                    printf("Not exist id:%d file:%s\n",id,name);
                }
            }
        }else if(strcmp(command,"render")==0){
            if(strcmp(method,"file")==0){
                renderFile(name,id);
            }
            if(strcmp(method,"size")==0){
                //render size
                int value = renderSize(name,id);
                if(value == notExist){
                    printf("id:%d file:%s not exist\n",id,name);

                }else{
                    if(value !=0){
                        printf("size:%d\n",value);
                    }else if(value ==0){
                        printf("size:%d\n",value);
                    }else{
                        printf("unknown error\n");
                    }
                }
            }
        }else if(strcmp(command,"replicate")==0){
            if(strcmp(method,"file")==0){
                //replicate
                askReplicate(name,id);
            }
        }else if(strcmp(command,"copy")==0){
            if(strcmp(method,"content")==0){
                int value = copyFile(name,id);
                if(value == success){
                    printf("Success\n");
                }else if(value == notExist){
                    printf("Not exist\n");
                }else{
                    printf("Unknown error\n");
                }
            }
        }else if(strcmp(command,"paste")==0){
            if(strcmp(method,"content")==0){
                int value = pasteFile(name,id);
                if(value==success){
                    printf("content copied to id:%d file:%s\n",id,name);
                }else if(value ==notExist){
                    printf("Target file not exist\n");
                }else{
                    printf("unknown error occured\n");
                }
            }
        }else if(strcmp(command,"import")==0){
            if(strcmp(method,"as")==0){
                int value = import_as(name,id);
                if(value == success){
                    printf("successfully imported as id:%d file:%s\n",id,name);
                }else if(value == isExist){
                    printf("id:%d file:%s already exists\n",id,name);
                }else if(value == notExist){

                    printf("No such file exists on this system\n");

                    perror("Error:");
                }else{
                    printf("Error occured\n");
                }
            }
        }else if(strcmp(command,"inject")==0){
            if(strcmp(method,"into")==0){
                int value = inject_into(name,id);
                if(value==success){
                    printf("Sucessfully injected data into id:%d file:%s\n",id,name);
                }else if(value==notExist){
                    printf("No such file exists on this system\n");
                }else{
                    printf("unknown error\n");
                }
            }
        }

        else{
            printf("Error\n");
        }
        
    }

}








//out-Systems

//logUse variable
void logUse(){
    printf("logged\n use.id:%d \n use.name=%s\n use.path:%s\n use.size:%ld\n",useId,useName,usePath,useSize);
    return;
}

//available Files as DATA
void logData(){
    
    char files[] = "files";
    char path[length];
    snprintf(path,sizeof(path),"%s/%s",internals_folder,"files");

    FILE *fp = fopen(path,"r");
    char tempname[length];
    int  id;
    int count =0;
    long int mem=0;
    if(fp!=NULL){
        while(fscanf(fp,"name=%s id=%d\n",tempname,&id)==2){
            long int size = getSize(tempname,id);
            printf("->id=%d file:%s ,%ld\n",id,tempname,size);
            count ++;
            mem+=size;
        }
        printf("\nmem:%ld",mem);
        printf("\nTotal:%d",count);
        printf("\n");
        fclose(fp);
    }else{
        printf("unknown error\n");
    }
}

//functions on LOG
void LOG(){
    
    FILE *fp = fopen(log_file,"r");
    if(fp == NULL){
        printf("No help file exist\n");
        return;
    }else{
        char ch;
        while((ch=fgetc(fp))!=EOF){
            printf("%c",ch);
        }
        fclose(fp);
        return;
    }
}


void HELP() {
    
    FILE *fp = fopen(help_file,"r");
    if(fp == NULL){
        printf("No help file found\n");
        return;
    }else{
        char  ch;
        
        while((ch=fgetc(fp))!=EOF){
            
            printf("%c",ch);
        }
        fclose(fp);
        return;
    }
}

void logWelcome(int t){
    int time=t;
    printf("  ____________ _____   ____ _______ _    _ \n");
    sleep(time);
    printf(" |___  /  ____|  __ \\ / __ \\__   __| |  | |\n");
    sleep(time);
    printf("    / /| |__  | |__) | |  | | | |  | |__| |\n");
    
    printf("   / / |  __| |  _  /| |  | | | |  |  __  |\n");
    sleep(time);
    printf("  / /__| |____| | \\ \\| |__| | | |  | |  | |\n");
    
    printf(" /_____|______|_|  \\_\\\\____/  |_|  |_|  |_|\n");
    sleep(time);
    
}




#define isVersion 87
#define noVersion 63
//
int checkVersion(char filename[],int id,char version[]){
    FILE  *fp = fopen(version_file,"r");
    char tempname[length],tempversion[length];
    int tempid;
    while(fscanf(fp,"name=%s id=%d version=%s\n",tempname,tempid,tempversion)==3){
        if(strcmp(filename,tempname)==0){
            if(id==tempid){
                if(strcmp(tempversion,version)==0){
                    fclose(fp);
                    return isVersion;
                }
            }
        }
    }
    fclose(fp);
    return noVersion;
}

// int createVersion(char filename[],int id){
//     int value = checkFile(filename,id);
//     if(value == isExist){
//         printf("Enter version number: ");
//         char version[length];
//         scanf("%s",version);
        
//         int check = checkVersion(filename,id,version);
//         if(check == isVersion){
//             return isVersion;
//         }else{
//             char path[length];
//             char ch;
//             snprintf(path,sizeof(path),"%s/%s_V%s",VERSION_folder,filename)
//         }
//     }
// }







///......................................For feed file API sample
int call_feed_file(char filename[],char id[]){
    int i_id= atoi(id);
    feedFile(filename,i_id);
}


#endif