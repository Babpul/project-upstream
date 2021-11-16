#include "user.h"
/*
이름    : mycd 함수
작성자  : 오규빈
기능    : 작업 디렉토리의 경로를 변경한다
받는값  : 경로 문자열
리턴값  : X
*/
void mycd (const char* path){
    if(path == NULL)
    {//path 인자가 없을시
        rear_dir_list_ptr == front_dir_list_ptr;//작업 디렉터리를 홈디렉터리로 설정
    }

    else if (path != NULL)
    {
        int i = 0;
        int first = 0;
        while(*(path+i) == '\0')
        {
          while(*(path+i) == '/')
          {
            if (first = 0)
            {
                first++;
            }
            else 
            {
                
            }
          } 
        }
           
        
    }
}