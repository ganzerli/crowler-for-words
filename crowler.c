#include "includes.c"                                           // stdio.h string.h stdlib.h curl.c
#include "html.c"          

#define _10MB sizeof(char) * 1024 * 1024 * 10

int main(void) {

    FILE * _fp;
    char* url = malloc(sizeof(char) * 256);                                 // URL max 256 chars
    char* buffer = malloc(sizeof(char) * 256);                              // I/O read max 256 chars
    const char _con_[] ={ 'h','t','t','p','s',':', '/', '/' };  
    char _char = 'a';

    strcat(url,_con_);                                                      // concat https:            // with URL
    printf("enter domain :");                                               // ask user 
    scanf("%s",buffer);                                                     // prompt scan domain
    strcat(url,buffer);                                        
    printf("domain choosen :%s\n",url);                                     // PRINT RESULT (complete domain name)

    //                                      MAKE HTML CALL
    char* data = malloc(63000);
    data = html_getHtml(data,url);                                          // http call from libc function
    //printf(data);
    free(buffer);                                                           // FREE

    // create  BUFFERs for trimmed document
    char *trimd_doc_buf = malloc(_10MB);
    char *links_buf = malloc(sizeof(char) * 1024 * 255);            
    char *callable_links = malloc(sizeof(char) * 1024 * 255);
                                                
    html_cut_script_style( trimd_doc_buf , data );                                      // CUT STYLE AND SCRIPT FROM THE DOCUMENT
    html_getLinks( url , callable_links , links_buf , trimd_doc_buf );                  // get content of all href= , https//domain can be added, need of another buffer
    html_filterHtml( trimd_doc_buf , trimd_doc_buf );                                   // leave only the text and cut all HTML tags

    printf("\n \n v \n v \n - > > putting in file:WORDSDUMP.txt\n");

    const char* filePath = "wordsdump.txt";

    _fp = fopen(filePath, "a");                                             // OPEN FILE in append mode
    if (_fp == NULL){                                                       // UNABLE to open file hence EXIT   
        printf("\nUnable to open '%s' file.\n", filePath);
        printf("Please check whether file exists and you have write privilege.\n");
        exit(EXIT_FAILURE);
    }

    fputs(trimd_doc_buf, _fp);                                              // Append data to file
    fclose(_fp);                                                            // CLOSE FILE
    _fp = NULL;
    //////////////// set a way to get clean string paramenters to call//////////////////////////////
    
    char * addr_to_call = malloc(63000);                                    // need of new BUFFER
    int * indexes = malloc( sizeof(int) * 500 );                            // and one BUFFER for the indexes
    int  idnx_z = 0;
    indexes[idnx_z] = 0;                                                    // set 1 st INDEX always at position[0]
    idnx_z = 1;                                                             // jump to 2nd place in the array to keep registering indexes
    int oneint  = 0;                                                        // keep last bite read after loop

    //until \0 read all chars and REPLACE the \0 with \n
    for(int i = 0 ; links_buf[i] != '\0'; i++){
        _char = links_buf[i];
        if(_char == '\n'){
            addr_to_call[i] = '\0';                                         // replace new line with \0 to have as multiple strings
            indexes[idnx_z] = i+1;
            idnx_z++;
        }else{
            addr_to_call[i] = _char;
        }
        // position of the last char
        oneint = i;                                                     
    }

    // pad the end of callable strings
    addr_to_call[oneint] = '\0';
    // create a random FLAG to stop the loop
    indexes[idnx_z] = 10201;
    
    printf(addr_to_call);
    printf("\n%s",links_buf);

    // call html, parse and save to file text from all links found in the first site
    char * data_ = malloc(63000);
    _char = '1';

    for(int i = 0; indexes[i] != 10201; i++){

        idnx_z = indexes[i];

        if(isalpha(addr_to_call[idnx_z]) ){        
            _char = '1';
        }else{
            _char = '0';
        }
        
        if( _char == '1' ){        

            printf("\ncalling :  %s\n" , &addr_to_call[idnx_z]);

            // HTML call
            data_ = html_getHtml(data_, &addr_to_call[idnx_z]);
            // parsing
            html_cut_script_style( trimd_doc_buf , data_ );
            html_getLinks( url, callable_links , links_buf , trimd_doc_buf );
            html_filterHtml( trimd_doc_buf , trimd_doc_buf );

            // OPEN FILE
            _fp = fopen(filePath, "a");

            if (_fp == NULL){
                printf("\nUnable to open '%s' file.\n", filePath);
                printf("Please check whether file exists and you have write privilege.\n");
                exit(EXIT_FAILURE);
            }

            // append data to FILE
            printf(trimd_doc_buf);
            fputs(trimd_doc_buf, _fp);            
            printf("\n");

            // CLOSE FILE
            fclose(_fp);
            // printf("\n9\n");
        }

    }//LOOP
    
    free(data_);
    free(trimd_doc_buf);
    free(links_buf);

    return 0;
}// MAIN