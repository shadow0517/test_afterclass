#ifndef _FILTER_WORDS_
#define _FILTER_WORDS_

#define MAX 20

#include"general_header.h"

/*每个单词存储的数据*/
typedef struct{
	char word[MAX];
	long word_number;
	int  word_value;
}WORD; 

typedef struct node{
	WORD *word_content;
	struct node *node_next;
}NODE;

typedef NODE * LIST_WORDS; 

typedef enum{
	FALSE,
	TRUE
}BOOL;

/*generate a linked list containing word information of the document content extracted.*/ 
LIST_WORDS FW_readin(void);

/*get the file point*/
FILE* acknowledge_fp();

/*creat a node according to the internal information the node*/
void DivideWordIntoList(LIST_WORDS list_words,char *arr[]);

/*check the compute value can match the node in the list*/
BOOL  match_node	(const LIST_WORDS list_words,const int comp_value);

/*compute the value of the word*/
int   comp_value	(char arr[]);

/*acquire a word from the file. Input:FILE point, Output:char * of a word*/
char *AcquireWordFromFile(FILE *fp);

/*check the error of the file opend*/
void  file_error    (FILE *fp);

/*creat a node to storage the word*/
NODE* creat_node	(char *arr[]);

/*skip the chars which are not the letter*/
BOOL  fp_skip_space (FILE *fp);

/*print the link_list generate from the English file*/
void  FW_print (LIST_WORDS list_words);

/*the function to print the result of the English file*/
void  file_print    ();





#endif
