//�������
#include "user.h"


//��ũ��
#define     COMMAND_SIZE            30      //command() �Լ����� �Է¹��� �� �ִ� ��ɾ��� ����


//�Լ�
/*
�̸�    : mypwd �Լ�
�ۼ���  : ������
���    : �۾� ���丮�� �����θ� ����Ѵ�
�޴°�  : X
���ϰ�  : X
*/
void mypwd(void)
{
    DIR_LIST *tmp_ptr = front_dir_list_ptr;

    if (tmp_ptr == NULL)
    {
        printf("pwd�� ���� linked list�� �������� �ʽ��ϴ�.\n");
        abort();
    }

    int first = 0;

    while (1)
    {
        printf("%s", tmp_ptr->name);

        if (tmp_ptr->next_ptr == NULL)
        {
            break;
        }
        else
        {
            tmp_ptr = tmp_ptr->next_ptr;
        }

        if (first == 0)
        {
            first++;
        }
        else
        {
            printf("/");
        }
    }

    return;
}


/*
�̸�    : myinode �Լ�
�ۼ���  : ������
���    : ������ inode�� ������ ����Ѵ�
�޴°�  : inode ��ȣ(���ڿ�)
���ϰ�  : X
*/
void myinode(const char *ptr)
{
    // ptr�� ����Ű�� ���ڿ��� ������ ��ȯ, �ùٸ� ������ �˻�
    int inode = 0;  // inode ��ȣ
    int figure = 0; // ptr�� ����Ű�� �ִ� ���ڿ��� �ڸ���
    int ptr_offset = 0;

    while (1)
    {
        if (*(ptr + ptr_offset) == 0)
        {
            break;
        }
        else
        {
            if ((*(ptr + ptr_offset) < 48) || (*(ptr + ptr_offset) > 57)) //���� ���ڰ� �ƴ� ���ڰ� �ִ� ���
            {
                printf("inode ��ȣ�� �߸��Ǿ����ϴ�.\n");
                return;
            }
            else
            {
                ptr_offset++;
                figure++;
            }
        }
    }

    ptr_offset = 0; //���� ������ ���� �ʱ�ȭ

    for (int i = (figure - 1); i >= 0; i--)
    {
        inode += (*(ptr + ptr_offset) - 48) * (int_pow(10, i));

        ptr_offset++;
    }

    if ((inode > 128) || (inode < 1)) // 1~128�� ������ �ƴ� ���
    {
        printf("inode ��ȣ�� �߸��Ǿ����ϴ�.\n");
        return;
    }

    //���� ����
    FILE *myfs;
    myfs = fopen("myfs.bin", "rb");
    if (myfs == NULL)
    {
        printf("myinode() �Լ� : ���� ���⿡ �����߽��ϴ�.\n");
        abort();
    }

    // myfs���� superblock ���� �б�
    SUPERBLOCK *sb_ptr = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
    fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
    fread(sb_ptr, sizeof(SUPERBLOCK), 1, myfs);

    // inode ��� ���� Ȯ���ϱ�
    unsigned mask;

    if (inode < 33) // inode_1�� ������ ����ִ� ���
    {
        mask = 1 << (inode - 1);
        if ((sb_ptr->inode_1 & mask) == 0)
        {
            printf("�ش� inode�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (inode < 65) // inode_2�� ������ ����ִ� ���
    {
        mask = 1 << ((inode - 32) - 1);
        if ((sb_ptr->inode_2 & mask) == 0)
        {
            printf("�ش� inode�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (inode < 97) // inode_3�� ������ ����ִ� ���
    {
        mask = 1 << ((inode - 64) - 1);
        if ((sb_ptr->inode_3 & mask) == 0)
        {
            printf("�ش� inode�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else // inode_4�� ������ ����ִ� ���
    {
        mask = 1 << ((inode - 96) - 1);
        if ((sb_ptr->inode_4 & mask) == 0)
        {
            printf("�ش� inode�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }

    // myfs���� inode list ���� �б�
    INODE *inode_data_ptr = (INODE *)malloc(sizeof(INODE)); // inode ������ ������ �޸� ������ ����Ű�� ������
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (inode - 1)), SEEK_SET);
    fread(inode_data_ptr, sizeof(INODE), 1, myfs);

    // inode ���� ����ϱ�
    //����
    printf("���� : ");
    if (inode_data_ptr->type == 0) //���� ������ 0�� ���
    {
        printf("���丮\n");
    }
    else //���� ������ 1�� ���
    {
        printf("����\n");
    }
    //��������
    printf("�������� : ");
    if (inode_data_ptr->year > 50)
    {
        printf("19%d/", inode_data_ptr->year);
    }
    else
    {
        printf("20%d/", inode_data_ptr->year);
    }
    printf("%02d/%02d ", inode_data_ptr->month, inode_data_ptr->date);
    printf("%02d:%02d:%02d\n", inode_data_ptr->hour, inode_data_ptr->minute, inode_data_ptr->second);

    //ũ��
    printf("ũ�� : %d\n", inode_data_ptr->size);

    // direct
    printf("���� ��� ��� : \n");

    if (inode_data_ptr->dir_1 != 0)
    {
        printf(" #0 ���� ������ ��� : %d\n", inode_data_ptr->dir_1 + 1);
    }
    if (inode_data_ptr->dir_2 != 0)
    {
        printf(" #1 ���� ������ ��� : %d\n", inode_data_ptr->dir_2 + 1);
    }
    if (inode_data_ptr->dir_3 != 0)
    {
        printf(" #2 ���� ������ ��� : %d\n", inode_data_ptr->dir_3 + 1);
    }
    if (inode_data_ptr->dir_4 != 0)
    {
        printf(" #3 ���� ������ ��� : %d\n", inode_data_ptr->dir_4 + 1);
    }
    if (inode_data_ptr->dir_5 != 0)
    {
        printf(" #4 ���� ������ ��� : %d\n", inode_data_ptr->dir_5 + 1);
    }
    if (inode_data_ptr->dir_6 != 0)
    {
        printf(" #5 ���� ������ ��� : %d\n", inode_data_ptr->dir_6 + 1);
    }
    if (inode_data_ptr->dir_7 != 0)
    {
        printf(" #6 ���� ������ ��� : %d\n", inode_data_ptr->dir_7 + 1);
    }
    if (inode_data_ptr->dir_8 != 0)
    {
        printf(" #7 ���� ������ ��� : %d\n", inode_data_ptr->dir_8 + 1);
    }

    // single indirect
    printf("���� ��� ��ȣ : %d\n", inode_data_ptr->indir);

    //���� �޸� �Ҵ� ���� �ݳ�
    free(inode_data_ptr);
    free(sb_ptr);

    //���� �ݱ�
    fclose(myfs);

    return;
}


/*
�̸�    : mydatablock �Լ�
�ۼ���  : ������
���    : ������ ������ ��Ͽ� ����ִ� ������ ����Ѵ�
�޴°�  : datablock ��ȣ(���ڿ�)
���ϰ�  : X
*/
void mydatablock(const char *ptr)
{
    // ptr�� ����Ű�� ���ڿ��� ������ ��ȯ, �ùٸ� ������ �˻�
    int datablock = 0;  // datablock ��ȣ
    int figure = 0; // ptr�� ����Ű�� �ִ� ���ڿ��� �ڸ���
    int ptr_offset = 0;

    while (1)
    {
        if (*(ptr + ptr_offset) == 0)
        {
            break;
        }
        else
        {
            if ((*(ptr + ptr_offset) < 48) || (*(ptr + ptr_offset) > 57)) //���� ���ڰ� �ƴ� ���ڰ� �ִ� ���
            {
                printf("datablock ��ȣ�� �߸��Ǿ����ϴ�.\n");
                return;
            }
            else
            {
                ptr_offset++;
                figure++;
            }
        }
    }

    ptr_offset = 0; //���� ������ ���� �ʱ�ȭ

    for (int i = (figure - 1); i >= 0; i--)
    {
        datablock += (*(ptr + ptr_offset) - 48) * (int_pow(10, i));

        ptr_offset++;
    }

    if ((datablock > 256) || (datablock < 1)) // 1~256�� ������ �ƴ� ���
    {
        printf("datablock ��ȣ�� �߸��Ǿ����ϴ�.\n");
        return;
    }

    //���� ����
    FILE *myfs;
    myfs = fopen("myfs.bin", "rb");
    if (myfs == NULL)
    {
        printf("mydatablock() �Լ� : ���� ���⿡ �����߽��ϴ�.\n");
        abort();
    }

    // myfs���� superblock ���� �б�
    SUPERBLOCK *sb_ptr = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
    fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
    fread(sb_ptr, sizeof(SUPERBLOCK), 1, myfs);

    // datablock ��� ���� Ȯ���ϱ�
    unsigned mask;

    if (datablock < (32 * 1 + 1)) // data_block_1�� ������ ����ִ� ���
    {
        mask = 1 << (datablock - 1);
        if ((sb_ptr->data_block_1 & mask) == 0)
        {
            printf("�ش� datablock�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 2 + 1)) // data_block_2�� ������ ����ִ� ���
    {
        mask = 1 << ((datablock - (32 * 1)) - 1);
        if ((sb_ptr->data_block_2 & mask) == 0)
        {
            printf("�ش� datablock�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 3 + 1)) // data_block_3�� ������ ����ִ� ���
    {
        mask = 1 << ((datablock - (32 * 2)) - 1);
        if ((sb_ptr->data_block_3 & mask) == 0)
        {
            printf("�ش� datablock�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 4 + 1)) // data_block_4�� ������ ����ִ� ���
    {
        mask = 1 << ((datablock - (32 * 3)) - 1);
        if ((sb_ptr->data_block_4 & mask) == 0)
        {
            printf("�ش� datablock�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 5 + 1)) // data_block_5�� ������ ����ִ� ���
    {
        mask = 1 << ((datablock - (32 * 4)) - 1);
        if ((sb_ptr->data_block_5 & mask) == 0)
        {
            printf("�ش� datablock�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 6 + 1)) // data_block_6�� ������ ����ִ� ���
    {
        mask = 1 << ((datablock - (32 * 5)) - 1);
        if ((sb_ptr->data_block_6 & mask) == 0)
        {
            printf("�ش� datablock�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 7 + 1)) // data_block_7�� ������ ����ִ� ���
    {
        mask = 1 << ((datablock - (32 * 6)) - 1);
        if ((sb_ptr->data_block_7 & mask) == 0)
        {
            printf("�ش� datablock�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 8 + 1)) // data_block_8�� ������ ����ִ� ���
    {
        mask = 1 << ((datablock - (32 * 7)) - 1);
        if ((sb_ptr->data_block_8 & mask) == 0)
        {
            printf("�ش� datablock�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }

    //datablock ���
    char *datablock_ptr = (char *)malloc(sizeof(char));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (datablock - 1)), SEEK_SET);

    for(int i = 0; i < 256; i++)
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

    printf("\n");
    
    //���� �޸� �Ҵ� ���� �ݳ�
    free(sb_ptr);
    free(datablock_ptr);

    //���� �ݱ�
    fclose(myfs);

    return;
}


/*
�̸�    : command �Լ�
�ۼ���  : ������
���    : linux ��ɾ �����Ѵ�
�޴°�  : X
���ϰ�  : X
*/
void command(void)
{
    //linux ��ɾ� �Է�
    char *command_ptr = (char *)malloc(sizeof(char) * COMMAND_SIZE);
    char tmp_char;
    int loop_char_num = 0;

    printf("linux ��ɾ �Է��ϼ��� : ");

    while((tmp_char = getchar()) != '\n')
    {
        *(command_ptr + loop_char_num) = tmp_char;

        loop_char_num++;
    }
    *(command_ptr + loop_char_num) = 0; //NULL ����

    //linux ��ɾ� ����
    system(command_ptr);

    return;
}


/*
�̸�    : �ŵ����� �Լ�
�ۼ���  : ������
���    : �ۼ��� �� ���ڷ� �ŵ������� �����Ѵ�
�޴°�  : �ŵ������� ������ �� ����
���ϰ�  : �ŵ����� ��� �����
*/
int int_pow(int a, int b) //a�� b �ŵ������� �����Ѵ�
{
    int value = 1;

    for(int i = 0; i < b; i++)
    {
        value *= a;
    }

    return value;
}


/*
�̸�    : mytree �Լ�
�ۼ���  : ������
���    : ���丮 ������ ����Ѵ�
�޴°�  : ������ ����� ���丮�� ������
���ϰ�  : X
*/
void mytree(const char *path_ptr)
{
    int tree_inode; //������ ����� ���丮�� inode ��ȣ�� ������ ����

    //��ɾ�κ��� ���޵� ���� ó��
    if(path_ptr == NULL) //���ڰ� �Էµ��� ���� ���
    {
        tree_inode = rear_dir_list_ptr->inode; //���� ���丮�� inode ����
    }
    else //���ڰ� �Էµ� ���
    {
        tree_inode = path_to_inode(path_ptr); //�ش� ���丮�� inode�� ����

        //��ΰ� �߸��� ��쿡 ���� ����ó��
        if(tree_inode == 0)
        {
            printf("�߸��� ����Դϴ�.\n");

            return;
        }
    }

    //���丮 ���� ���
    

    return;
}


/*
�̸�    : path_to_inode �Լ�
�ۼ���  : ������
���    : �����θ� �޾Ƽ� �ش� ���丮�� inode ��ȣ�� �����Ѵ�
�޴°�  : 
���ϰ�  : X
*/
int path_to_inode(const char *path_ptr)
{
    int inode = 1; //inode ��ȣ�� ������ ����. �⺻�� 1(root)
    
    int tmp_datablock; //datablock ��ȣ�� ������ ����
    
    INODE *inode_ptr = (INODE *)malloc(sizeof(INODE)); //inode�� ����ų ������

    char *tmp_cmd_string_ptr = (char *)malloc(sizeof(char) * 8); //path���� ������ ���丮���� ����ų ������
    
    char *tmp_dir_string_ptr = (char *)malloc(sizeof(char) * 8); //���丮�� datablock���� ������ ���丮���� ����ų ������
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //���丮�� datablock���� ������ inode ��ȣ�� ����ų ������

    FILE *myfs;
    myfs = fopen("myfs.bin", "rb");

    path_ptr++; //��Ʈ(/) �Ѿ��

    while(1)
    {
        //path���� ���ڿ� �и�
        if(sscanf(path_ptr ,"%[^/\\0]", tmp_cmd_string_ptr) != 1) //tmp_string_ptr���� ���丮���� ����ǰ�, �� ������ NULL�� ��
        {
            break;
        }

        path_ptr = path_ptr + (strlen(tmp_cmd_string_ptr) + 1);

        //inode�� ���丮�� �̵�, �ش� datablock���� �̵�
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (inode - 1)), SEEK_SET);
        fread(inode_ptr, sizeof(INODE), 1, myfs);

        tmp_datablock = (int)(inode_ptr->dir_1 + 1);
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (256 * (tmp_datablock - 1)), SEEK_SET);

        //datablock�� ���丮�� ����, tmp_cmd_string_ptr�� ��
        int diff_error = 0;
        for(int i = 0; i < (inode_ptr->size) / (8 + sizeof(int)); i++)
        {
            fread(tmp_dir_string_ptr, 8, 1, myfs);
            
            if(strcmp(tmp_dir_string_ptr, tmp_cmd_string_ptr) == 0) //�� ���ڿ��� ���� ���
            {
                fread(tmp_inode_ptr, sizeof(int), 1, myfs);
                inode = *tmp_inode_ptr;

                break;
            }
            else //�� ���ڿ��� �ٸ� ���
            {
                fread(tmp_inode_ptr, sizeof(int), 1, myfs); //inode ��ȣ �ǳʶٱ�
                diff_error++;
            }
        }

        if(diff_error == ((inode_ptr->size) / (8 + sizeof(int))))
        {
            free(tmp_cmd_string_ptr);
            free(inode_ptr);
            free(tmp_dir_string_ptr);
            free(tmp_inode_ptr);

            fclose(myfs);

            return 0;
        }
    }

    free(tmp_cmd_string_ptr);
    free(inode_ptr);
    free(tmp_dir_string_ptr);
    free(tmp_inode_ptr);

    fclose(myfs);

    return inode;
}

