
// just to test
void html_printHtml(char* data, char* url){
    // manage http response
    data = handle_url(url);
    if(data) {
        printf("%s\n", data);
        free(data);
    }
}
// manage HTML call with libcurl functions
char* html_getHtml(char* data, char* url){

    if(isalpha(url[0])  && isalpha(url[1]) ){
        data = handle_url(url);
    }else{
        data[0] = '\0';
    }
    return data;
}

void html_cut_script_style(char* resultbuffer ,char* data){

    char c;
    signed int i = 0;
    signed int i_result = 0;
    char ignore_is_on = 'N';
    char regc = '#';

    while(data[i]!='\0') {

        if(data[i] == '<'){
            if(data[i+1] == 's'){
           //                   0123456 7
           //'s' --> CHECK FOR  <SCRIPT'/0'
                regc = data[ i + 7 ];                                   // get last char of word script from char array
                data[ i + 7 ] = '\0';                                   // replace it with '\0' to compare
                if(strcmp(&data[i], "<script") == 0 ){
                    // script found
                    // printf("%s  found at position: %d\n" , &data[i], (int)i );
                    ignore_is_on = 'Y';
                    data[ i + 7 ] = regc;                               // replace char back
                    i += 7;                                             // jump over word <script[_]
                }else{
                    data[ i + 7 ] = regc;                               // replace char
                }
                //                   012345 6
                //'s' --> CHECK FOR  <style'/0'
                regc = data[ i + 6 ];                                   // get last char of word script from char array
                data[ i + 6 ] = '\0';                                   // replace it with '\0' to compare
                if(strcmp(&data[i], "<style") == 0 ){
                    // script found
                    // printf("%s  found at position: %d\n" , &data[i], (int)i );
                    ignore_is_on = 'Y';
                    data[ i + 6 ] = regc;                               // replace char back
                    i += 6;                                             // jump over word <script[_]
                }else{
                    data[ i + 6 ] = regc;                               // replace char
                }
            }
            if(data[i+1] == '/'){
            //                   01234567  8
            //'/' --> CHECK FOR  </script [>]   <<--- '\0'
                regc = data[ i + 8 ];                                   // get last char of word script from char array
                data[ i + 8 ] = '\0';                                   // replace it with '\0' to compare
                if(strcmp(&data[i], "</script") == 0 ){
                    // script found
                    // printf("%s  found at position: %d\n" , &data[i], (int)i );
                    ignore_is_on = 'N';
                    data[ i + 8 ] = ' ';                                // hack to avoid '<' remaining
                    i += 8;                                             // jump over /script>[_]
                }else{
                    data[ i + 8 ] = regc;                               // replace char back
                }

                //                   0123456  7
                //'/' --> CHECK FOR  </style [>]   <<--- '\0'
                regc = data[ i + 7 ];                                   // get last char of word script from char array
                data[ i + 7 ] = '\0';                                   // replace it with '\0' to compare
                if(strcmp(&data[i], "</style") == 0 ){
                    // script found
                    // printf("%s  found at position: %d\n" , &data[i], (int)i );
                    ignore_is_on = 'N';
                    data[ i + 7 ] = ' ';                                // hack to avoid '<' remaining
                    i += 7;                                             // jump over /style>[_]
                }else{
                    data[ i + 7 ] = regc;                               // replace char back
                }   
            }
        }
        if(ignore_is_on == 'N'){
            resultbuffer[i_result] = data[i]; 
            i_result++;
        }
       i++;    
    }   
    // pad end of result buffer
    resultbuffer[i_result] = '\0';
    // printf(resultbuffer);
}

void html_filterHtml(char* resultbuffer ,char* data){

    char c;
    signed int i = 0;
    signed int i_result = 0;
    char ignore_is_on = 'N';
    
    while(data[i]!='\0') {

        if(data[i] == '<'){
            ignore_is_on = 'Y';
        }
        
        if(data[i] == '>'){
            if(data[i+1] == '<'){
                //keep ignoring
                ignore_is_on = 'Y';
            }else{
                i++;                                                    // get i over '>'    
                ignore_is_on = 'N';                                     // SET IGNORE TO 'N'
                resultbuffer[i_result] = ' ';                           // split also words in html menu, info, listing...
                i_result++; 
            }
        }

        if(ignore_is_on == 'N'){
           resultbuffer[i_result] = data[i];                            // save char
           i_result++;
        }
       i++;
    }

    // PAD END OF WRITTEN DATA
    resultbuffer[i_result-1] = '\0';
    resultbuffer[i_result] = '\0';

//    printf("%s",resultbuffer);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                  char * doc_buffer          char * doc_buffer
// FUNCTION TO cut off spaces of the html document buffer, redirectly in the buffer ["buffer buf bf buffer"] , ["buffer  buf      bf         '\n'    buffer"]
    char lastch = 'a';
    signed int offset = 0;

    for(int i = 0 ; resultbuffer[i] != '\0'; i++){

        if(isalpha(resultbuffer[i])){
            lastch = resultbuffer[i];
            //printf("%c",resultbuffer[i]);
            resultbuffer[offset] = lastch;
            offset++;
        }else{
            if(lastch == ' '){
                //nothing
            }else{
                // printf(" ");
                lastch = ' ';
                resultbuffer[offset] = lastch;
                offset++;
            }
        }
    }
    resultbuffer[offset] = '\0';
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////
}  //
/////

void html_getLinks(char* url ,char* callable_links, char* resultbuffer ,char * data){

    char c;
    signed int i = 0;
    signed int i_result = 0;
    char ignore_is_on = 'N';
    char regc = '#';
    
    while(data[i]!='\0') {

        if(data[i] == '<'){
            ignore_is_on = 'Y';
        }
        if(data[i] == '>'){
            if(data[i+1] == '<'){
                //keep ignoring
                ignore_is_on = 'Y';
            }else{
                i++;                                                    // get i over '>'    
                ignore_is_on = 'N';                                     // SET IGNORE TO 'N'
            }
        }
        if(ignore_is_on == 'Y'){
            // check for links
            if(data[i] == 'h'){

                regc = data[ i + 5 ];                                   // get last char of word href=["] from char array
                data[ i + 5 ] = '\0';                                   // replace it with           '\0' to compare
                if(strcmp(&data[i], "href=") == 0 ){
                    data[ i + 5 ] = regc;                               // replace char back
                    i += 5;                                             // jump over word href=["] link/link/link "
                    // get the link
                    // GET ALSO OVER open QUOTES'  "  '
                    i++;
                    while(data[i] != '\"'){
                        //printf("%c",data[i]);
                        resultbuffer[i_result] = data[i];               // save char
                        i_result++;
                        i++;
                    }
                    resultbuffer[i_result] = '\n';
                    i_result++;
                }else{
                    data[ i + 5 ] = regc;                               // replace char
                }
            }
        }
        i++;
    }

    // PAD END OF WRITTEN DAT
    i_result++;resultbuffer[i_result] = '\0';
    // printf("%s",resultbuffer);
   
//FUNCTON FOR RETURNING A CLEAN CHAR*[WITH CALLABLE COMPLETE ADDRESSES https://www.domainame.sh/info/?query=?abc+?%11001100/'\n'...'\0']////////////////////////////////////////
    char ch = 'a';
    char lastch = 'a';
    int offset = 0;
    short signed int eslasches = 0;                                     // i like very much spanish accent

    for(int i = 0; resultbuffer[i] != '\0'; i++){
        ch = resultbuffer[i];
        if(ch == '\n'){                                                 // new line occours
            callable_links[ offset ] = '/';     offset++;           
            lastch = '0';
        }
        if(ch == 'h'){                                                  // char can be [h]ttp and the last char was new line
            if(lastch == '0'){
                lastch = 'h';
            }
        }
        if(ch == '/'){                                                  // when get to / notate as one slash
            eslasches++;
            if( resultbuffer[i+1] == '/'){                              // if next is also / ../[/] then notate as well
                eslasches++;
            }
        }
        if(lastch == '0'){                                              // when new line found
            if (eslasches > 1){                                         // begin with [//] ----> //www.abc.com
                callable_links[ offset ] = 'h'; offset++;
                callable_links[ offset ] = 't'; offset++;
                callable_links[ offset ] = 't'; offset++;
                callable_links[ offset ] = 'p'; offset++;               // add https before
                callable_links[ offset ] = 's'; offset++;
                callable_links[ offset ] = ':'; offset++;
                lastch = '/';                                           // set custom flag not to destroy all mechanics...
            }
            if(eslasches == 1){
                for(int i = 0; url[i] != '\0'; i++){
                    callable_links[ offset ] = url[i]; offset++;
                }
                lastch = '/';                                           // set custom flag not to destroy all mechanics...
            }
        }

        callable_links[ offset ] = ch ; offset++;
        eslasches = 0;
    }
    callable_links[offset] = '\0';
// FUNCTION END /////////////////////////////////////////////// FUNCTION END // */*//***/*/*/***/*///*/***/*//**/*/////////

    // return the initial buffer
    for(int i = 0; callable_links[i] != '\0'; i++){                 
        resultbuffer[i] = callable_links[i];
    }

    resultbuffer[offset] = '\0';
    //printf(resultbuffer);
}
/////////////////////////////////////////////////////////////// END OF LIST ########################################################################################