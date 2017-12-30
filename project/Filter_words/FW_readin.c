#include"Filter_words.h"

LIST_WORDS FW_readin(void)
{
	FILE *fp;
	static LIST_WORDS *list_words;
	LIST_WORDS *node_return_judge;
	
	fp=acknowledge_fp();

	list_words=creat_node(fp);
	/*add the new word into the linked list*/
	for(int i=0;feof(fp)!=0;i++)
	{
		node_return_judge=classify_word(fp);
		if(node_return_judge==NULL)
			continue;
		else
			sort_list(list_words,newnode);

		fp_skip_space(fp);
	}
	fclose(fp);
	return list_words;
}



/*acknowledge the filename need read*/
FILE *acknowledge_fp()
{
	static char filename_read[MAX];

	printf("\033[40;41mplease input the name of file\n\033[0m");
	scanf("%s",filename_read);
	fp=fopen(filename_read,"r+");
	file_error(fp);
	return fp;
}
/*creat a node from the file to save the word and return the point of the node*/
NODE *creat_node(FILE *fp)
{
	NODE *node_new;
	node_new=(NODE *)malloc(sizeof(NODE));
	fscanf(fp,"%s",node_new->word_content->word);
	file_error(fp);
	node_new->word_content->word_number++;
	node_new->word_content->word_value=comp_value(node_new->\
	word_content->word);
	node_new->node_next=NULL;

	fp_skip_space(fp);
	return node_new; }
/*creat a node according to the internal information the node*/
NODE *classify_word(FILE *fp)
{
	int comp_value_newnode;
	char array_temp[MAX];
	NODE *newnode;

	fscanf(fp,"%s",array_temp);
	file_error(fp);
	comp_value_newword=value_word(array_temp);
	fp_skip_space(fp);
	
	if(match_node(list_words,comp_temp)==TRUE)
		return NULL;
	else
	{
		newnode=(NODE *)malloc(sizeof(NODE));
		newnode->word_content->word_number++;
		newnode->word_content->word_value=comp_value_newnode;
		newnode->node_next==NULL;
		for(int i=0;i<MAX;i++)
			newnode->word_content->word[i]=arr[i];

	}
	return newnode;
}
/*input a word as a array and return the value computed*/
int comp_value(const char arr[])
{
	int comp_value=0;
	
	for (int i=0;arr[i]!='\0';i++)
	{
		if(arr[i]>='A'&&arr[i]<='Z')
			arr[i]+=32;
		comp_value+=(arr[i]-96)*(i+1);
	}
	
	return comp_value;
}
/*input the point of file and skip the spaces to read the next word*/
void fp_skip_space(FILE *fp)
{
	char char_temp;
	for(int i=0;char_temp==' ';i++)
		char_temp=fgetc(fp);
	fseek(fp,-1L,1);
}
/*if cthe value computed of a word equel to one node of the list, \
 *lassify and add it into the linked list
 */
BOOL match_node(const LIST_WORDS *list_words,const int comp_value)
{
	NODE *node_temp=list_words;

	for(int i=0;;i++)
	{
		if(comp_value==node_temp->word_content->word_value)
		{
			node_temp->word_content->word_number++;
			return TRUE;
		}
		if(node_temp->node_next==NULL)
			return FALSE;
		node_temp=node_temp->node_next;
	}
}
/*sort the nodes of linked list as the value computed in the process\
 *of creating the linked list
 */
void sort_list(const LIST_WORDS *list_words,const NODE *newnode)
{
	NODE *node_temp=list_words;
	for(int i=0;;i++)
	{
		if(newnode->word_content->word_value<=node_temp->\
		word_content->word_value)
		{
			newnode->node_next=node_temp->node_next;
			node_temp->node_next=newnode;
		}
		node_temp=node_temp->node_next;
	}
}

		

