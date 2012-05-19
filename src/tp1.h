#ifndef _TP1_H_
#define _TP1_H_ 

#define GRAMMAR   1
#define AUTOMATHA 2

#define G_TUPLES      0
#define G_NON_TERMS   1
#define G_TERMS       2
#define G_START       3
#define G_PRODUCTIONS 4

#define A_STATE_SHAPE_START      1
#define A_STATE_SHAPE_END        2
#define A_STATE_NAME_START       3
#define A_STATE_NAME_END         4
#define A_STATE_LABEL_START      5
#define A_STATE_LABEL_END        6
#define A_STATE_END              7

#define A_TRANS_MATCH            8
#define A_TRANS_LABEL_START      9
#define A_TRANS_LABEL_END        10


extern int parsingGrammar;
extern int state; 
extern int parsingAutomatha;

void run(int len, char ** args);

void start_adts(int mode);

#endif

