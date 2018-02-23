#ifndef _STREAM_PARSE_H_
#define _STREAM_PARSE_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define WF_SB_EMPTY  	1
#define WF_SB_NOTEMPTY 	0

#define WF_SB_FULL 		1
#define WF_SB_NOTFULL	0

#define WF_GET FAIL		1
#define WF_GET_OK		0

#define WF_INSERT_OK 0

static inline int stream_buffer_is_empty(stream_buffer *sb);

/*
 * 数据流缓冲区
 * 实现为一个环形队列
 */
typedef struct {
	uint32_t  capacity;
	uint32_t  head;
	uint32_t  tail;
	char     *buf;
} stream_buffer;

static inline stream_buffer *stream_buffer_create(uint32_t capacity)
{
	stream_buffer *sb; 

	sb = (stream_buffer *)malloc(sizeof(stream_buffer));
	if (!sb)
		return NULL;

	sb->buf	= (char *)malloc(sizeof(char)*capacity);
	if (!sb->buf)
		return NULL;

	sb->capacity      = capacity;
	sb->head 		  = sb->tail = 0;
	return sb;
}

void stream_buffer_destroy(stream_buffer *sb)
{
	if(sb->buf)
		free(sb->buf);
	if(sb)
		free(sb);
}

static inline int stream_buffer_insert_word(stream_buffer *sb,
						char *word, int len)
{
	if (stream_buffer_empty_size(sb) <= len)
		return WF_SB_FULL;
	else {
		for(int i = 0; i < len; i++) {
			sb->buf[sb->tail] = word[i];
			sb->tail = (sb->tail+1)%sb->capacity;
		}
		sb->buf[sb->tail] = '\0';
		return WF_INSERT_OK;
	}
}

static int stream_buffer_get_word(stream_buffer *sb, char *word)
{
	if(stream_buffer_is_empty(sb))
		return WF_GET_FAIL;
	else
	{
		do {
			word[i]  = sb->buf[sb->head];
			sb->head = (sb->head+1)%sb->capacity;
		} while (!stream_buffer_is_empty(sb) && word[i] != '\0');
		return WF_GET_OK;
	}
}

static inline int stream_buffer_is_empty(stream_buffer *sb)
{
	if(sb->tail == sb->head)
		return WF_EMPTY;
	else 
		return WF_NOTEMPTY;
}

static inline int stream_buffer_empty_size(stream_buffer *sb)
{
	return ((sb->tail - sb->head + sb->capacity) % sb->capacity);
}

/*
 * 数据输入
 * 实现方式可以为从一个文件或者一个网络，读取相应内容
 * 考虑文本特殊情况包括：各种标点符号，字符大小写，换行，空行等的处理
 */
char *stream_input_parse(char *path);

#endif
