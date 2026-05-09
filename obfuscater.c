#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#define _0xf4faS puts
char pathh[500] = "";
char fullSourceToCopy[10000] = "";
typedef struct{
        char original[256];
        char obfuc_obj[256];
    }FuncMappingObj;
// Variablenname vor dem String finden
// Beispiel: char* msg = "hello"
//                 ^^^
void getVarName(char* sourceCode, char* returnStrStr, char* varName) {
    char* pos = returnStrStr - 1;
    

    while (*pos == ' ' || *pos == '=') pos--;

    char* end = pos + 1;
    while (*pos != ' ' && *pos != '*') pos--;
    pos++;
    int len = end - pos;
    strncpy(varName, pos, len);
    varName[len] = '\0';
}
void replaceInString(char* str, char* oldWord, char* newWord) {
    char buffer[10000];
    char *pos = strstr(str, oldWord);
    if (!pos) return;
    strncpy(buffer, str, pos - str);
    buffer[pos - str] = '\0';
    strcat(buffer, newWord);
    strcat(buffer, pos + strlen(oldWord));
    strcpy(str, buffer);
}
void replaceTheStrings(char* sourceCode)
{
    char* start = sourceCode;
    char* needle = "\"";

    while (1)
    {
        char* returnStrStr = strstr(sourceCode, needle);

        if (returnStrStr == NULL)
        {
            _0xf4faS("END");
            fflush(stdout);
            break;
        }

        // rückwärts schauen ob der string in einem funktionsaufruf ist ( oder ,
        char* check = returnStrStr - 1;
        while (check > start && (*check == ' ' || *check == '\t')) check--;

        if (*check == '(' || *check == ',')
        {
            printf("\n[+] Skipping function argument string.");
            sourceCode = returnStrStr + 1;
            continue;
        }

        // variablenname holen und auf _vars prüfen hierr
        char varName[256] = {0};
        getVarName(start, returnStrStr, varName);

        if (strstr(varName, "_vars") == NULL)
        {
            printf("\n[+] No _vars pattern, skipping.");
            sourceCode = returnStrStr + 1;
            continue;
        }

        printf("\n[+] String pattern found! Obfuscating..");

        // string zwischen den anführungszeichen einlesen
        char* scndDone2 = returnStrStr + 1;
        char foundStr[2560] = {0};
        int counter = 0;

        while (1)
        {
            if (*scndDone2 == '"') break;
            char tmp2[2] = {*scndDone2, '\0'};
            strcat(foundStr, tmp2);
            scndDone2++;
            counter++;
        }

        if (counter == 0)
        {
            sourceCode = returnStrStr + 1;
            continue;
        }

        // string xor verschlüsseln
        char encrypted[5500] = "{";
        for (int x = 0; x < counter; x++)
        {
            char byte[16];
            sprintf(byte, "0x%02X,", foundStr[x] ^ 0xFF);
            strcat(encrypted, byte);
        }
        strcat(encrypted, "0x00}");

        // "BlaBla" → {0x...,0x00};\n_0xDECRYPT(varName)
        char oldLine[512];
        char newLine[1024];
        sprintf(oldLine, "char* %s = \"%s\";", varName, foundStr);

        sprintf(newLine,
        "char %s[] = %s;\n_0xDECRYPT(%s);",
        varName,
        encrypted,
        varName);
        replaceInString(start, oldLine, newLine);

        sourceCode = returnStrStr + 1;
    }

    strcpy(fullSourceToCopy, start);
}


void searchforElement(char textparam[5000]){
    char defines[5000] = "";
    strcat(defines, "void _0xDECRYPT(char* str) {\n    for (int i = 0; str[i] != 0; i++) {\n        str[i] ^= 0xFF;\n    }\n}\n\n");
    printf("\n[+] Searching for functions to rename.. [+]");
    char* text = textparam;
    //separater output buffer damit original nicht zerstört wird
    char output[10000];
    strcpy(output, textparam);
    int counter = 0;
    char *ptr = text;
    
    char* needle = "(";
    char allOriginalFuncts[200][234];
    memset(allOriginalFuncts, 0, sizeof(allOriginalFuncts));
    fflush(stdout);
    FuncMappingObj ourStruct[200];
    memset(ourStruct, 0, sizeof(ourStruct));
    while (1){
        
        char* res = strstr(ptr, needle); /*search for function brackets*/

        if (res == NULL)
        {
            _0xf4faS("END");
            fflush(stdout);
            break;

        }else{     
            *res = '\0';
            counter++;
            char currentFunc[234];
            char *currentPos = res - 1;
            char obfuscatedStr[250]; 
            bool alreadyStored = false;
            
            int index = 0;
            while (1){
                
                if (*currentPos == ' '){
                    printf("\nBREAK");
                    currentPos++;
                    // neues strcpy(text, currentPos) entfernt hat vorher den ganzen Buffer zerstört
                    break;
                }else{
                   strcpy(currentFunc, currentPos);
                   
                   
                   for (int x = 0; x < 200; x++)
                   {
                    if (allOriginalFuncts[x][0] == '\0')
                    {
                        index = x;
                        break;
                    }
                    if (strcmp(allOriginalFuncts[x], currentFunc) == 0)
                    {
                        alreadyStored = true;
                        //hier trotzdem auch wenn es schon gestored ist es ersetzen
                        break;
                    }

                   }
                   
                   
                }
                currentPos--;

            }
            if (!alreadyStored){
                        strcpy(allOriginalFuncts[index], currentFunc);
                        char charset[] = "0123456789" "abcdefghijklmnopqrstuvwxyz" "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
                        int length = strlen(currentFunc) -1;
                        
                        
                        size_t nchars = strlen(charset);
                        for (size_t i = 0; i < length; i++)
                        {
                            obfuscatedStr[i] = charset[rand() % nchars];
                        }
                        obfuscatedStr[length] = '\0';
                        printf("\n[+] Setting new environment for old '%s'->'_0x%s'", currentFunc, obfuscatedStr);
                        char defineLine[512];
                        sprintf(defineLine, "#define _0x%s %s \n", obfuscatedStr, currentFunc);
                        strcat(defines, defineLine);
                        
                        char full[500];
                        sprintf(full, "_0x%s", obfuscatedStr);
                        
                        replaceInString(output, currentFunc, full);
                        strcpy(ourStruct[counter].original, currentFunc);
                        strcpy(ourStruct[counter].obfuc_obj, full);
                        
                        //hier vor dem memmove noch einen obfuscated string erstellen der mit '_' beginnt. 
                   }
                else{
                    for (int x = 0; x < 200; x++)
                    {
                        if (strcmp(ourStruct[x].original, currentFunc) == 0)
                        {
                            
                            replaceInString(output, currentFunc, ourStruct[x].obfuc_obj);
                            break;
                        }
                    }
                    
                    
                }
            if (counter == 0)
            {
                printf("\n[+] No functions found. Are you sure it's the right file? [+]");
                exit(0);
            }
            
            printf("\n[+] Result .. -> %s [+]", currentFunc);
            //hier nach strings suchen
            
            //als nächstes das wort in die liste abspeichern und schlussendlich abgleichen ob das wort schon in der list ist, wenn nicht obfuscaten und in den source code reinkopieren
        }
         
        ptr = res + strlen(needle);
        printf(".");
    }
    replaceTheStrings(output);
    FILE* fileOpennn = fopen(pathh, "w");
    fprintf(fileOpennn, "%s\n", defines);
    fprintf(fileOpennn, "%s", output);
    fclose(fileOpennn);
    printf("Functions %s found in source code-> %d times\n", needle, counter);
}

void main(int argc, char* argv[]){
    printf("\n[+] Searching for functions to rename..\n");
    srand(time(NULL));
    char sc[10000];
    strcpy(pathh, argv[1] );
    char src[10000] = "";
    char* welcomeTxt = "[+] Fast C-Source Code obfuscater [+]";
    bool fewArgs = false;

    argc < 2?fewArgs = true:printf("\n[+] Inspecting '%s'\n", argv[1]);

    if (fewArgs == true){
        printf("\n[+] Obfuscation failed -> less arguments than expected [+]");
        printf("\nUsage -> ./obfuscater.exe FILEPATHHERE.c");
        exit(0);
    }
    printf("%s\n", welcomeTxt);
    //hier checken ob FILE existiert
    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL){
        printf("File was not found.");
        exit(0);
    }else{
        if (strstr(argv[1], ".c")){
            printf("File found");
            src[0] = '\0';
            while (fgets(sc, sizeof(sc), fp)) {
                strcat(src, sc);
            }
            searchforElement(src);
            
            //hier auch noch funktion einfügen die nach " sucht und wenn es es gefunden hat, strings mit xor verschlüsselt und " eine hilfsfunktion die dann zur laufzeit entschlüsselt
            /*
            Heißt:
            1. Oben 1 Entschlüsselungs/Decriptions Funktion
            2. Jeden string verschlüsseln mit selben Xor KEY  (UND GANZ WICHTIG jede variable muss _varc am ende haben damit wir wissen was eine string variable ist und was nicht)
            3. Nach jeder Verschlüsselung ein mal darunter entschlüsselungsfunktion aufrufen
            
            
            */

        }else{
            printf("File found, but the file is not a c-source code file ( e.g. example.c ) .\n");
            exit(0);
        }       

    }
    fclose(fp);
    exit(0);
    //als nächstes die chars rückwärts speichern bis zum leerzeichen und neu defined in den source code schreiben
}