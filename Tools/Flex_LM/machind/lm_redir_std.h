/**************************************************************************************************
* Copyright (c) 2015-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/


#ifndef MAINWIN_FLEXNET_H_LM_REDIR_STD_H_
#define MAINWIN_FLEXNET_H_LM_REDIR_STD_H_

#include <stdio.h>

#ifdef PC
FILE* lm_flex_stdout();
FILE* lm_flex_stderr();
FILE* lm_flex_stdin();
#else
#define lm_flex_stdout() stdout
#define lm_flex_stderr() stderr
#define lm_flex_stdin() stdin
#endif


#endif /* MAINWIN_FLEXNET_H_LM_REDIR_STD_H_ */
