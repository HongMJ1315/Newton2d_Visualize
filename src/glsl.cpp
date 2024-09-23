#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "glsl.h"
//Global varibles: the program object IDs and the number of program objects created by us,
GLuint   myProgObjID;
int          numProgObj = 0;

/*-----------------------------------------------------------------------
 * Procedure to read the source codes of a shader.
 *  filename: name of the source code file (text file).
 * Return a string containg the source codes.
 */
char *read_source_codes(char *filename){
    FILE *fptr = fopen(filename, "r");
    char *buffer = 0;
    unsigned int  size;

    if(fptr == NULL){
        fprintf(stderr, " Source code file: %s doesn't exist. \n", filename);
        exit(0);
    }
    else{
        fseek(fptr, 0, SEEK_END);
        size = ftell(fptr);
        buffer = (char *) malloc(size + 1);
        //move the file pointer to the head of the source code file.
        rewind(fptr);
        //Read the source code.
        size = fread(buffer, sizeof(char), size, fptr);
        buffer[size] = '\0';
        fclose(fptr);
    }
    return buffer;
}

/*-----------------------------------------------------------------------------------
 * Local procedure to check compile errors.
 *   obj: shader ID.
 */
void print_shader_info_log(GLenum obj){
    int     status;
    char   infoLog[1024];

    glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE){
        glGetShaderInfoLog(obj, 1024, NULL, infoLog);
        fprintf(stderr, "Shader compile error: %s\n", infoLog);
        exit(-1);
    }
    fprintf(stderr, "Shader compiling is successful. \n");
}

/*-----------------------------------------------------------------------------------
 * Local procedure to check link errors.
 *   obj: program object ID.
 */
void print_prog_info_log(GLenum obj){
    int     status;
    char   infoLog[1024];

    glGetProgramiv(obj, GL_LINK_STATUS, &status);
    if(status == GL_FALSE){
        glGetProgramInfoLog(obj, 1024, NULL, infoLog);
        fprintf(stderr, "Shader link error: %s\n", infoLog);
        exit(-1);
    }
    fprintf(stderr, "Shader linking is successful. \n");
}

/*---------------------------------------------------------------------------------------------------
 * A global procedure declared in "GLSLSetup.h".
 *  1. Create vertex shadre, fragment shader, program object
 *  2. Fetch the source codes of the shaders.
 *  3. Compile, attach, and link the shaders.
 * Keep the program object ID in  myProgObjID[0] and return.
 */
int setGLSLshaders(char *vertexShaderFileName, char *fragmentShaderFileName){
    GLuint    vertexShader = 0, fragmentShader = 0;
    char *VSsourceCodes, *FSsourceCodes;


    //create a program object
    myProgObjID = glCreateProgram();

    //create the shaders
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    //read the source codes of the shaders
    VSsourceCodes = read_source_codes(vertexShaderFileName);
    FSsourceCodes = read_source_codes(fragmentShaderFileName);

    //Print out the source codes for error checking.
    fprintf(stderr, "Vertex   Shader Source : [\n%s\n]\n", VSsourceCodes);
    glShaderSource(vertexShader, 1, (char **) &VSsourceCodes, NULL);
    glCompileShader(vertexShader);
    print_shader_info_log(vertexShader);

    fprintf(stderr, "Fragment ShaderSources: [\n%s\n]\n", FSsourceCodes);
    glShaderSource(fragmentShader, 1, (char **) &FSsourceCodes, NULL);
    glCompileShader(fragmentShader);
    print_shader_info_log(fragmentShader);
    //Load the source codes into the shaders

    //attach shaders to program
    glAttachShader(myProgObjID, vertexShader);
    glAttachShader(myProgObjID, fragmentShader);

    //Link the shaders
    glLinkProgram(myProgObjID);
    //Check link errors
    print_prog_info_log(myProgObjID);

    //Replace the default program object with ours.
    // glUseProgram(myProgObjID);
    numProgObj = 1;
    std::cout << "Program ID" << ((unsigned int) myProgObjID) << std::endl;
    return myProgObjID;

}

