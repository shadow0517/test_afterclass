#include "data_store.h"

/*
 * 双向链表的实现形式
 */
typedef struct list_node {
	data_store_object *obj;
	struct list_node  *next;
	struct list_node  *prev;
} data_store_list_node;

typedef struct {
	data_store_list_node *head;
	data_store_list_node *tail;
	uint32_t              count;
} data_store_list;

/*list operations here!*/
static data_store_list_node *node_find(data_store_list *ds_list, char *word)
{
	data_store_list_node *node_temp;

	/*字符串比较，一般先比较字符串长度，然后使用strncmp函数*/
	for (node_temp = ds_list->head->next; node_temp; node_temp = node_temp->next)
		if (!strcmp(word, node_temp->obj->word))
			return node_temp;

	return NULL;
}

static data_store_list_node *node_insert(char *word)
{
	data_store_list_node *node_insert;
	char *word_insert;

	node_insert = (data_store_list_node *)calloc(1, sizeof(data_store_list_node));
	if (!node_insert) 
		return NULL;

	node_insert->obj = (data_store_object *)calloc(1, sizeof(data_store_object));
	if (!node_insert->obj) 
		return NULL;
	
	word_insert = (char *)calloc(1, sizeof(char)*(strlen(word)+1));
	if (!word_insert)
		return NULL;
	strcpy(word_insert, word);

	node_insert->obj->word  = word_insert;
	node_insert->obj->count = 1;
	return node_insert;
}

static inline void node_free(data_store_list_node *node)
{
	if (node) {
		if(node->obj) {
			if(node->obj->word)
				free(node->obj->word);
			free(node->obj);
		}
		free(node);
	}
}

static inline void node_count_inc(data_store_list_node *node)
{
	node->obj->count++;
}

static inline void node_exchange(data_store_list_node *node1, data_store_list_node *node2)
{
	char *word_temp;
	int	  count_temp;

	word_temp 		  = node1->obj->word;
	node1->obj->word  = node2->obj->word;
	node2->obj->word  = word_temp;

	count_temp 		  = node1->obj->count;
	node1->obj->count = node2->obj->count;
	node2->obj->count = count_temp;

}

data_store *data_store_create(void)
{
	data_store      	 *ds      	= NULL;
	data_store_list 	 *ds_list 	= NULL;
	data_store_list_node *head_node = NULL;

	ds_list = (data_store_list *)calloc(1, sizeof(data_store_list));
	if (!ds_list)
		return NULL;
	
	head_node = (data_store_list_node *)calloc(1,sizeof(data_store_list_node));
	if(!head_node)
		return NULL;

	ds_list->head 	= ds_list->tail   = head_node;
	ds_list->count  = 0;
	head_node->prev = head_node->next = NULL;
	head_node->obj  = NULL;

	ds = (data_store *)calloc(1, sizeof(data_store));
	if (!ds) {
		free(ds_list);
		return NULL;
	}

	ds->type = DATA_STORE_TYPE_LIST;
	ds->priv = ds_list;

	return ds;
}

void data_store_destroy(data_store *ds)
{
	data_store_list_node *node_temp;
	data_store_list		 *ds_list;

	ds_list = (data_store_list *)ds->priv;
	for (node_temp = ds_list->tail; ds_list->tail; ds_list->tail = node_temp) { 
		node_free(ds_list->tail);
		node_temp = node_temp->prev;
	}
	if (ds_list)
		free(ds_list);
	if (ds)
		free(ds);
}

int data_store_insert_count(data_store *ds, char *word)
{
	data_store_list_node *node = NULL;
	data_store_list 	 *ds_list;

	ds_list = (data_store_list *)ds->priv;
	node = node_find(ds_list, word);
	if (node) {
		node_count_inc(node);
		return 0;
	}
	else {
		node = node_insert(word);
		if (!node)
			return WF_WORD_INSERT_FAIL;

		node->prev 	  		= ds_list->tail;
		ds_list->tail->next = node;
		node->next	  		= NULL;
		ds_list->tail 		= node;
		ds_list->count		++;
	}
}

void data_store_get_max_count(data_store *ds, data_store_object *set, int index)
{
	data_store_list_node *node_temp;
	data_store_list		 *ds_list;

	ds_list   = (data_store_list *)ds->priv;
	node_temp = ds_list->head->next;
	for (int i = 0; i < index; i++) {
		if (node_temp != NULL) {
			set[i].word  = node_temp->obj->word;
			set[i].count = node_temp->obj->count;
			node_temp 	 = node_temp->next;
		}
		else {
			set[i].word  = NULL;
			set[i].count = -1;
		}
	}
}

int data_store_sort(data_store *ds)
{
	data_store_list_node *node_i, *node_j;
	data_store_list		 *ds_list;

	ds_list = (data_store_list *)ds->priv;
	if (ds_list->count == 0)
		return WF_DATA_STORE_EMPTY;

	for (node_i = ds_list->head->next; node_i; node_i = node_i->next) {
		for (node_j = node_i->next; node_j; node_j = node_j->next)
		{
			if (node_j->obj->count > node_i->obj->count)
				node_exchange(node_i, node_j);
		}
	}
	return 0;
}

void data_store_print_max_count(data_store_object *set, char *path)
{
	system("clear");
	printf("\033[40;32m******当前文档%s中单词数前十的单词统计******\n\033[0m",path);
	printf("\033[47;30m单词\t\t\t\t\t\t单词数\n\033[0m");
	for (int i = 0; i < WF_WORD_PRINT_NUMBER && set[i].word; i++)
		printf("\033[40;31m%-30s\t\t\t%5d\n\033[0m",set[i].word,set[i].count);
	printf("\n");
	printf("\033[47;30m点击回车继续\033[0m\n");
	getchar();
	return;
}

data_store_object *data_store_object_array_creat(uint32_t object_number)
{
	data_store_object *set;

	set = (data_store_object *)calloc(1, sizeof(data_store_object)*object_number);
	if (!set)
		return NULL;
	return set;
}
	
void data_store_object_array_destroy(data_store_object *set, uint32_t object_number)
{
	free(set);
}
	
	
