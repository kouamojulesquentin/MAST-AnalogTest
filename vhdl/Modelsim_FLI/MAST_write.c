/*
 * Copyright 1991-2012 Mentor Graphics Corporation
 *
 * All Rights Reserved.
 *
 * THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF 
 * MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS SUBJECT TO LICENSE TERMS.
 */

#include <mti.h>
#include <stdio.h>
#include <stdlib.h>

FILE *outfile;

/* Convert a VHDL String array into a NULL terminated string.
 * The caller is responsible for freeing the returned string.
 */

 
FILE **log_files=NULL;
static int n_log_files = 1;
static int registred_log_files = 0;
 
static char *get_string( mtiVariableIdT id )
{
    char *      buf;
    int         len;
    mtiTypeIdT  type;

    type = mti_GetVarType(id);
    len  = mti_TickLength(type);
    buf  = malloc( sizeof(char) * (len+1) );
    mti_GetArrayVarValue(id, buf);
    buf[len] = 0;
    return buf;
}

/* ********** C code for VHDL init_target procedure. ********** */
void init_target (
  mtiVariableIdT  filename       /* IN string         */
)
{
    char * string_val;
    string_val = get_string(filename);
    mti_PrintFormatted( "Opening exchange file %s\n", string_val );
    outfile = fopen(string_val,"w+");
    free( string_val );
}

void set_n_log_files ( int new_n_log_files
)
{
 int i;
 n_log_files = new_n_log_files;
 log_files = (FILE **) calloc(sizeof(FILE *),n_log_files);
 for (i=0;i<n_log_files;i++)
   log_files[i]=NULL;
 registred_log_files = 0;
}

int init_log_file (
  mtiVariableIdT  filename       /* IN string         */
)
{
    char * string_val;
    string_val = get_string(filename);
    mti_PrintFormatted( "Opening %d-th log file (out of max : %d) %s\n", 
                         registred_log_files+1, n_log_files,string_val );
    log_files[registred_log_files] = fopen(string_val,"w+");
    free( string_val );
    return registred_log_files++;
}


/* ********** C code for VHDL in_params procedure. ********** */
void write_string (
  mtiVariableIdT  vhdl_array       /* IN string         */
)
{
    char * string_val;
    if (outfile ==NULL)
     {
      mti_PrintFormatted("WARNING: target non initialized\n");
      outfile = fopen("pippo.dat","w+");
     }
    string_val = get_string(vhdl_array);
    mti_PrintFormatted("Writing string \"%s\" to file\n",string_val);
    fprintf(outfile,"%s",string_val);
    free( string_val );
}

void flush_string (
)
{
    fprintf(outfile,"\n");
    fflush(outfile);
}

/* ********** C code for VHDL in_params procedure. ********** */
void write_log_line (
   int n_log,
  mtiVariableIdT  vhdl_array       /* IN string         */
)
{
    char * string_val;
    if  ((n_log > n_log_files) && (log_files[n_log] ==NULL))
     {
      mti_PrintFormatted("WARNING: log file %d non initialized\n",n_log);
      outfile = fopen("pippo.dat","w+");
     }
    string_val = get_string(vhdl_array);
    mti_PrintFormatted("Writing string \"%s\" to log %d\n",string_val,n_log);
    fprintf(log_files[n_log],"%s\n",string_val);
    fflush(log_files[n_log]);
    free( string_val );
}

