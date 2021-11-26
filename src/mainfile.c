//�������
#include "user.h"


//�۾� ���丮 ��θ� ������ linked list
DIR_LIST *front_dir_list_ptr = NULL;
DIR_LIST *rear_dir_list_ptr = NULL;


/*
�̸�    : main �Լ�
�ۼ���  : ������
���    : X
�޴°�  : X
���ϰ�  : X
*/
int main(void)
{
    //pwd linked list ����
    front_dir_list_ptr = (DIR_LIST *)malloc(sizeof(DIR_LIST));
    front_dir_list_ptr->inode = 1;
    front_dir_list_ptr->name = "/";
    front_dir_list_ptr->next_ptr = NULL;
    rear_dir_list_ptr = front_dir_list_ptr;

    //test
    FILE *myfs = fopen("myfs", "wb");

    int inode;
    int datablock;
    int dir_inode;
    int *dir_inode_ptr = &dir_inode;
    INODE *inode_ptr = (INODE *)malloc(sizeof(INODE)); //inode�� ����ų ������
    char *dir_list_ptr = (char *)malloc(sizeof(char) * 9);
    
    char *minus_ptr = (char *)malloc(sizeof(char));
    *minus_ptr = -1;

    // /
    //inode �б�
    inode = 1;

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (inode - 1)), SEEK_SET);
    inode_ptr->size = 5 * (8 + sizeof(int));
    inode_ptr->dir_1 = 0;
    inode_ptr->type = 0;
    fwrite(inode_ptr, sizeof(INODE), 1, myfs);

    datablock = 1;

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (256 * (datablock - 1)), SEEK_SET);
    sprintf(dir_list_ptr, "%s", "..");
    dir_list_ptr[2] = 0;
    fwrite(dir_list_ptr, 8, 1, myfs);
    dir_inode = 1;
    fwrite(dir_inode_ptr, sizeof(int), 1, myfs);

    sprintf(dir_list_ptr, "%s", ".");
    dir_list_ptr[1] = 0;
    fwrite(dir_list_ptr, 8, 1, myfs);
    dir_inode = 1;
    fwrite(dir_inode_ptr, sizeof(int), 1, myfs);

    sprintf(dir_list_ptr, "%s", "mnt");
    dir_list_ptr[3] = 0;
    fwrite(dir_list_ptr, 8, 1, myfs);
    dir_inode = 2;
    fwrite(dir_inode_ptr, sizeof(int), 1, myfs);

    sprintf(dir_list_ptr, "%s", "test");
    dir_list_ptr[4] = 0;
    fwrite(dir_list_ptr, 8, 1, myfs);
    dir_inode = 3;
    fwrite(dir_inode_ptr, sizeof(int), 1, myfs);

    sprintf(dir_list_ptr, "%s", "result2");
    dir_list_ptr[7] = 0;
    fwrite(dir_list_ptr, 8, 1, myfs);
    dir_inode = 5;
    fwrite(dir_inode_ptr, sizeof(int), 1, myfs);

    fwrite(minus_ptr, sizeof(char), 1, myfs);

    //result2
    inode = 5;

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (inode - 1)), SEEK_SET);
    inode_ptr->size = 2 * (8 + sizeof(int));
    inode_ptr->dir_1 = 4;
    inode_ptr->type = 1;
    fwrite(inode_ptr, sizeof(INODE), 1, myfs);

    datablock = 5;

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (256 * (datablock - 1)), SEEK_SET);

    sprintf(dir_list_ptr, "%s", "..");
    dir_list_ptr[2] = 0;
    fwrite(dir_list_ptr, 8, 1, myfs);
    dir_inode = 1;
    fwrite(dir_inode_ptr, sizeof(int), 1, myfs);

    sprintf(dir_list_ptr, "%s", ".");
    dir_list_ptr[1] = 0;
    fwrite(dir_list_ptr, 8, 1, myfs);
    dir_inode = 5;
    fwrite(dir_inode_ptr, sizeof(int), 1, myfs);

    fwrite(minus_ptr, sizeof(char), 1, myfs);

    // mnt
    //inode �б�
    inode = 2;

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (inode - 1)), SEEK_SET);
    inode_ptr->size = 3 * (8 + sizeof(int));
    inode_ptr->dir_1 = 1;
    inode_ptr->type = 0;
    fwrite(inode_ptr, sizeof(INODE), 1, myfs);

    datablock = 2;

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (256 * (datablock - 1)), SEEK_SET);

    sprintf(dir_list_ptr, "%s", "..");
    dir_list_ptr[2] = 0;
    fwrite(dir_list_ptr, 8, 1, myfs);
    dir_inode = 1;
    fwrite(dir_inode_ptr, sizeof(int), 1, myfs);

    sprintf(dir_list_ptr, "%s", ".");
    dir_list_ptr[1] = 0;
    fwrite(dir_list_ptr, 8, 1, myfs);
    dir_inode = 2;
    fwrite(dir_inode_ptr, sizeof(int), 1, myfs);

    sprintf(dir_list_ptr, "%s", "result");
    dir_list_ptr[6] = 0;
    fwrite(dir_list_ptr, 8, 1, myfs);
    dir_inode = 4;
    fwrite(dir_inode_ptr, sizeof(int), 1, myfs);
    
    fwrite(minus_ptr, sizeof(char), 1, myfs);

    //test(���ϸ�)
    inode = 3;

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (inode - 1)), SEEK_SET);
    inode_ptr->size = 2 * (8 + sizeof(int));
    inode_ptr->dir_1 = 2;
    inode_ptr->type = 0;
    fwrite(inode_ptr, sizeof(INODE), 1, myfs);

    datablock = 3;

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (256 * (datablock - 1)), SEEK_SET);

    sprintf(dir_list_ptr, "%s", "..");
    dir_list_ptr[2] = 0;
    fwrite(dir_list_ptr, 8, 1, myfs);
    dir_inode = 1;
    fwrite(dir_inode_ptr, sizeof(int), 1, myfs);

    sprintf(dir_list_ptr, "%s", ".");
    dir_list_ptr[1] = 0;
    fwrite(dir_list_ptr, 8, 1, myfs);
    dir_inode = 3;
    fwrite(dir_inode_ptr, sizeof(int), 1, myfs);

    fwrite(minus_ptr, sizeof(char), 1, myfs);

    //result
    inode = 4;

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (inode - 1)), SEEK_SET);
    inode_ptr->size = 2 * (8 + sizeof(int));
    inode_ptr->dir_1 = 3;
    inode_ptr->type = 1;
    fwrite(inode_ptr, sizeof(INODE), 1, myfs);

    datablock = 4;

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (256 * (datablock - 1)), SEEK_SET);

    sprintf(dir_list_ptr, "%s", "..");
    dir_list_ptr[2] = 0;
    fwrite(dir_list_ptr, 8, 1, myfs);
    dir_inode = 2;
    fwrite(dir_inode_ptr, sizeof(int), 1, myfs);

    sprintf(dir_list_ptr, "%s", ".");
    dir_list_ptr[1] = 0;
    fwrite(dir_list_ptr, 8, 1, myfs);
    dir_inode = 4;
    fwrite(dir_inode_ptr, sizeof(int), 1, myfs);

    fwrite(minus_ptr, sizeof(char), 1, myfs);

    fclose(myfs);

    //testend

    //��
    shell();

    return 0;
}


/*
�̸�    : �� �Լ�
�ۼ���  : ������
���    : ������Ʈ�� ���� ��ɾ �Է¹޾� ���� �Լ��� ȣ���Ѵ�(�� ��� ����)
�޴°�  : X
���ϰ�  : X
*/
void shell(void)
{
    //��ɾ� ó���� ���� �����͵�
    char **com_sep_ptr = (char **)malloc(sizeof(char *) * COM_SEP_NUM); //4���� �����͸� ����ų �� �ִ� ������ ������

    for(int i = 0; i < COM_SEP_NUM; i++)
    {   
        *(com_sep_ptr + i) = (char *)malloc(COM_SEP_SIZE); //�� �����Ϳ� ���� �޸� �Ҵ�
    }

    //�� ���, ��ɾ� ó��
    while(1)
    {
        //myfs ���� ���翩�� Ȯ��
        int exist = 0; //������ �������� �ʴ� ��� 0, �����ϴ� ��� 1 ����
        FILE *myfs_exist;
        if((myfs_exist = fopen("myfs", "rb")) != NULL) //������ �������� �ʴ� ���
        {
            exist = 1;
        }
        fclose(myfs_exist);

        //�� ���
        if(exist == 0) //myfs ������ �������� �ʴ� ���
        {
            printf("$ ");
        }
        else //myfs ������ �����ϴ� ���
        {
            printf("[");
            mypwd(); //mypwd() �Լ��� ��� ���
            printf(" ]$ ");
        }

        //�����Ͱ� ����Ű�� �޸� ���� �ʱ�ȭ
        for(int i = 0; i < COM_SEP_NUM; i++)
        {   
            for(int j = 0; j < COM_SEP_SIZE; j++)
            {
                *(*(com_sep_ptr + i) + j) = 0;
            }
        }

        //��ɾ� �Է�
        char char_tmp;
        int com_ptr_num = 0; //������ ������ �ε���
        int com_ptr_char_num = 0; //������ ������ ����Ű�� ������ �ε���

        rewind(stdin); //���� ����

        while((char_tmp = getchar()) != '\n')
        {
            if(char_tmp != ' ') //���� ���ڰ� �ƴ� ���
            {
                *(*(com_sep_ptr + com_ptr_num) + com_ptr_char_num) = char_tmp;
            }
            else //���� ������ ���
            {
                *(*(com_sep_ptr + com_ptr_num) + com_ptr_char_num) = 0; //NULL ���� ����
                com_ptr_num++;
                com_ptr_char_num = 0;
                continue;
           }

            com_ptr_char_num++;
        }
        *(*(com_sep_ptr + com_ptr_num) + com_ptr_char_num) = 0;

        //�Է� ������ �ʱ�ȭ, ��ó��
        char **com_tmp_ptr = (char **)malloc(sizeof(char *) * COM_SEP_NUM); //4���� �����͸� ����ų �� �ִ� ������ ������

        for(int i = 0; i < (com_ptr_num + 1); i++)
        {
            *(com_tmp_ptr + i) = *(com_sep_ptr + i);
        }
        for(int i = (com_ptr_num + 1); i < COM_SEP_NUM; i++)
        {
            *(com_tmp_ptr + i) = NULL;
        }

        //��ɾ� �Լ� ȣ��
        if(!strcmp("mymkfs", *(com_tmp_ptr)))
        {
            mymkfs();
        }
        else if((!strcmp("myls", *(com_tmp_ptr))) && (exist == 1))
        {
            //myls �Լ�
        }
        else if((!strcmp("mycat", *(com_tmp_ptr))) && (exist == 1))
        {
            //mycat �Լ�
        }
        else if((!strcmp("myshowfile", *(com_tmp_ptr))) && (exist == 1))
        {
            //myshowfile �Լ�
        }
        else if((!strcmp("mypwd", *(com_tmp_ptr))) && (exist == 1))
        {
            mypwd();
            printf("\n");
        }
        else if((!strcmp("mycd", *(com_tmp_ptr))) && (exist == 1))
        {
            //mycd �Լ�
        }
        else if((!strcmp("mycp", *(com_tmp_ptr))) && (exist == 1))
        {
            //mycp �Լ�
        }
        else if((!strcmp("mycpto", *(com_tmp_ptr))) && (exist == 1))
        {
            //mycpto �Լ�
        }
        else if((!strcmp("mycpfrom", *(com_tmp_ptr))) && (exist == 1))
        {
            //mycpfrom �Լ�
        }
        else if((!strcmp("mymkdir", *(com_tmp_ptr))) && (exist == 1))
        {
            //mymkdir �Լ�
        }
        else if((!strcmp("myrmdir", *(com_tmp_ptr))) && (exist == 1))
        {
            //myrmdir �Լ�
        }
        else if((!strcmp("myrm", *(com_tmp_ptr))) && (exist == 1))
        {
            //myrm �Լ�
        }
        else if((!strcmp("mymv", *(com_tmp_ptr))) && (exist == 1))
        {
            mymv(*(com_tmp_ptr + 1), *(com_tmp_ptr + 2));
        }
        else if((!strcmp("mytouch", *(com_tmp_ptr))) && (exist == 1))
        {
            //mytouch �Լ�
        }
        else if((!strcmp("myinode", *(com_tmp_ptr))) && (exist == 1))
        {
            myinode(*(com_tmp_ptr + 1));
        }
        else if((!strcmp("mydatablock", *(com_tmp_ptr))) && (exist == 1))
        {
            mydatablock(*(com_tmp_ptr + 1));
        }
        else if((!strcmp("mystate", *(com_tmp_ptr))) && (exist == 1))
        {
            //mystate �Լ�
        }
        else if((!strcmp("mytree", *(com_tmp_ptr))) && (exist == 1))
        {
            mytree(*(com_tmp_ptr + 1));
        }
        else if((!strcmp("command", *(com_tmp_ptr))) && (exist == 1))
        {
            command();
        }
        else if(!strcmp("exit", *(com_tmp_ptr)))
        {
            printf("Bye....\n\n");
            return;
        }
        else if(exist == 0)
        {
            printf("���� �ý����� �������� �ʽ��ϴ�.\n");
        }
        else
        {
            printf("�߸��� ��ɾ��Դϴ�.\n");
        }

        printf("\n");
    }

    return;
}