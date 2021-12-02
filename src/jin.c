#include "user.h"
/*
이름 : stringtoint함수
작성자 : 양인석
기능 : 문자열 정수로변환
받는값 : 문자열
출력값 : 정수
*/
int stringtoint(char *byte)
{
    int n;
    if(strlen(byte) == 1)
    {
        n = (*(byte) - 48);
    }
    else if(strlen(byte) == 2)
    {
        n = ((*(byte) - 48) * 10) + (*(byte + 1) - 48);
    }
    else if(strlen(byte) == 3)
    {
        n = ((*(byte) - 48) * 100) + ((*(byte + 1) - 48) * 10) + (*(byte + 2) - 48);
    }
    else if(strlen(byte) == 4)
    {
        n = ((*(byte) - 48) * 1000) + ((*(byte + 1) - 48) * 100) + ((*(byte + 2) - 48) * 10) + (*(byte + 3) - 48);
    }
    return n;
}

/*
이름    : myls 함수
작성자  : 이우진
기능    : 파일 목록 자세히 보기
받는값  : X
리턴값  : X
*/
void myls(const char *ptr)
{
    int inode = 0;  // inode 번호
    int num = 0; // ptr이 가리키고 있는 문자열의 자릿수
    int ptr_a = 0;

    while (1)
    {
        if (*(ptr + ptr_a) == 0)
        {
            break;
        }
        else
        {
            if ((*(ptr + ptr_a) < 48) || (*(ptr + ptr_a) > 57)) // 정수 문자가 아닌 문자가 있는 경우
            {
                printf("inode 번호가 잘못되었습니다.\n");
                return;
            }
            else
            {
                ptr_a++;
                num++;
            }
        }
    }

    ptr_a = 0; // 변수 재사용을 위해 초기화

    for (int i = (num - 1); i >= 0; i--)
    {
        inode += (*(ptr + ptr_a) - 48) * (int_pow(10, i));

        ptr_a;
    }

    if ((inode < 1) || (inode > 128)) // 1~128인 정수가 아닌 경우
    {
        printf("inode 번호가 잘못되었습니다.\n");
        return;
    }

    // 파일 열기
    FILE *myfs;
    myfs = fopen("myfs", "rb");

    if(myfs == NULL)
    {
        printf("파일 열기에 실패했습니다.\n");
        abort();
    }

    // superblock 정보 읽기
    SUPERBLOCK *sb_ptr = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
    fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
    fread(sb_ptr, sizeof(SUPERBLOCK), 1, myfs);

    // inode 사용 여부 확인하기
    unsigned mask;

    if (inode < 33) // inode_1에 정보가 들어있는 경우
    {
        mask = 1 << (inode - 1);
        if ((sb_ptr->inode_1 & mask) == 0)
        {
            printf("해당 inode는 사용 중이 아닙니다.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (inode < 65) // inode_2에 정보가 들어있는 경우
    {
        mask = 1 << ((inode - 32) - 1);
        if ((sb_ptr->inode_2 & mask) == 0)
        {
            printf("해당 inode는 사용 중이 아닙니다.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (inode < 97) // inode_3에 정보가 들어있는 경우
    {
        mask = 1 << ((inode - 64) - 1);
        if ((sb_ptr->inode_3 & mask) == 0)
        {
            printf("해당 inode는 사용 중이 아닙니다.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else // inode_4에 정보가 들어있는 경우
    {
        mask = 1 << ((inode - 96) - 1);
        if ((sb_ptr->inode_4 & mask) == 0)
        {
            printf("해당 inode는 사용 중이 아닙니다.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }


    while(inode < 128 + 1 ) // 파일 업데이트 시간, 파일 유형, inode 번호, 파일 크기
    {
        // inode list 정보 읽기
        INODE *inode_ptr = (INODE *)malloc(sizeof(INODE)); 
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (inode - 1)), SEEK_SET);
        fread(inode_ptr, sizeof(INODE), 1, myfs);


        // 생성일자
        printf("    생성일자 : ");
        
        if (inode_ptr->year > 50)
        {
            printf(" %d/", inode_ptr->year);
        }
        else
        {
            printf(" %d/", inode_ptr->year);
        }
        printf("%d/%d ", inode_ptr->month, inode_ptr->date);
        printf("%d:%d:%d", inode_ptr->hour, inode_ptr->minute, inode_ptr->second);


        // 종류
        printf("종류 : ");

        if (inode_ptr->type == 0) // 파일 종류 0 = 디렉토리
        {
            printf(" 디렉토리");
        }
        else // 파일 종류 1 = 일반 파일
        {
            printf(" 파일");
        }


        // inode 번호

        printf("    inode번호 : %d", inode);


        // 크기
        printf("    크기 : %d\n", inode_ptr->size);

        free(inode_ptr);

        inode++;
    }
    
    free(sb_ptr);
    fclose(myfs);

    return;
}

/*
이름    : mycat 함수
작성자  : 양인석
기능    : 파일 내용 출력 명령어
받는값  : 파일이름
리턴값  : X
*/
void mycat(char *givenname)
{
    //예외처리
    if (givenname == NULL)
    {
        printf("파일명이 필요합니다.\n");
        return;
    }

    if (strlen(givenname) > 7)
    {
        printf("문자열이 너무 깁니다. (최대 7글자)\n");

        return;
    }

    FILE *myfs;
    myfs = fopen("myfs", "rb+");
    
    //현재 디렉터리 아이노드 받기
    int saveinode = rear_dir_list_ptr->inode; //현재 디렉토리의 inode 번호 저장
    //현재 디렉토리의 inode 정보 불러오기
    INODE *presenti_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinode - 1), SEEK_SET);
    fread(presenti_data, sizeof(INODE), 1, myfs);
    
    //파일명이 같은 파일이 있는지 검사
    char *filename = (char *)malloc(sizeof(char) * 8);
    int *inodenumber = (int *)malloc(sizeof(int));
    int saveinumber;
    unsigned count = 0;

    int dir_file_num = presenti_data->size / (8 + sizeof(int));

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * presenti_data->dir_1), SEEK_SET);
    for (int i = 0; i < dir_file_num; i++)
    {
        fread(filename, 8, 1, myfs);
        fread(inodenumber, sizeof(int), 1, myfs);

        if (strcmp(givenname, filename) == 0)
        {
            saveinumber = *inodenumber;
            break;
        }
        else
        {
            count++;
        }
    }
    //읽을 파일의 아이노드 받기
    INODE *i_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinumber - 1), SEEK_SET);
    fread(i_data, sizeof(INODE), 1, myfs);

    //이진파일내용 읽어서 출력
    char *datablock_ptr = (char *)malloc(sizeof(char));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (i_data-> dir_1)), SEEK_SET);
    for(int i = 0; i < i_data-> size; i++)
    {
        fread(datablock_ptr, sizeof(char), 1, myfs);

        if(*datablock_ptr == -1)
        {
            break;
        }
        else
        {
            printf("%c", *datablock_ptr);
        }
    }
    
    free(presenti_data);
    free(filename);
    free(inodenumber);
    free(i_data);
    free(datablock_ptr);
    return;
}

/*
이름    : myshowfile 함수
작성자  : 양인석
기능    : 파일 지정된 부분 출력 명령어
받는값  : 바이트시작값, 바이트종료값, 파일명
리턴값  : X
*/
void myshowfile(char *startbyte, char *endbyte, char *givenname)
{
    //문자열 정수로 바꾸기
    int intstartbyte = stringtoint(startbyte);
    int intendbyte = stringtoint(endbyte);
    printf("%d\n%d\n", intstartbyte, intendbyte);

    //예외처리
    if (givenname == NULL)
    {
        printf("파일명이 필요합니다.\n");
        return;
    }

    if (strlen(givenname) > 7)
    {
        printf("문자열이 너무 깁니다. (최대 7글자)\n");

        return;
    }

    FILE *myfs;
    myfs = fopen("myfs", "rb+");
    
    //현재 디렉터리 아이노드 받기
    int saveinode = rear_dir_list_ptr->inode; //현재 디렉토리의 inode 번호 저장
    //현재 디렉토리의 inode 정보 불러오기
    INODE *presenti_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinode - 1), SEEK_SET);
    fread(presenti_data, sizeof(INODE), 1, myfs);
    
    //파일명이 같은 파일이 있는지 검사
    char *filename = (char *)malloc(sizeof(char) * 8);
    int *inodenumber = (int *)malloc(sizeof(int));
    int saveinumber;
    unsigned count = 0;

    int dir_file_num = presenti_data->size / (8 + sizeof(int));

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * presenti_data->dir_1), SEEK_SET);
    for (int i = 0; i < dir_file_num; i++)
    {
        fread(filename, 8, 1, myfs);
        fread(inodenumber, sizeof(int), 1, myfs);

        if (strcmp(givenname, filename) == 0)
        {
            saveinumber = *inodenumber;
            break;
        }
        else
        {
            count++;
        }
    }
    //읽을 파일의 아이노드 받기
    INODE *i_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinumber - 1), SEEK_SET);
    fread(i_data, sizeof(INODE), 1, myfs);

    //특정부분 읽은 후 출력
    char *datablock_ptr = (char *)malloc(sizeof(char));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (i_data-> dir_1)) + (intstartbyte - 1), SEEK_SET);
    for(int i = intstartbyte; i < intendbyte; i++)
    {
        fread(datablock_ptr, sizeof(char), 1, myfs);

        if(*datablock_ptr == -1)
        {
            break;
        }
        else
        {
            printf("%c", *datablock_ptr);
        }
    }
    
    free(presenti_data);
    free(filename);
    free(inodenumber);
    free(i_data);
    free(datablock_ptr);
    return;
}