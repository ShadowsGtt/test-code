#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include "cJSON.h"

int main()
{
#if 0
    char *test1 = "hello,world,gaotian";
    cJSON *json = cJSON_Parse(test1);
    printf("test1:%s\n",test1);
    char *out = cJSON_Print(json);

    printf("out:%s\n",out);
#endif
    cJSON *js = cJSON_CreateObject();
    cJSON_AddStringToObject(js,"name","gaotian");
    cJSON_AddStringToObject(js,"sex","man");
    cJSON_AddNumberToObject(js,"age",20);
    printf("1:%s\n",js->child->string);
    printf("2:%s\n",js->child->valuestring);
    printf("3:%s\n",js->child->next->string);
    printf("4:%s\n",js->child->next->valuestring);
    printf("3:%s\n",js->child->next->next->string);
    printf("4:%s\n",js->child->next->next->valuestring);
    printf("4:%f\n",js->child->next->next->valuedouble);
    char *out = cJSON_Print(js);
    printf("out:\n%s",out);
    printf("\nlen:%lu\n",strlen(out));
}
