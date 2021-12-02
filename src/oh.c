#include "user.h"


/*
�̸�    : mycd �Լ�
�ۼ���  : ���Ժ�
���    : �۾� ���丮�� ��θ� �����Ѵ�
�޴°�  : ��� ���ڿ�
���ϰ�  : X
*/
void mycd (char path[])
{
    if(path == NULL)
    {//path ���ڰ� ������
        rear_dir_list_ptr = front_dir_list_ptr;//�۾� ���͸��� Ȩ���͸��� ����
    }
    else if (path != NULL)
    {
        int first = 0;//ù / �߶󳻱�
        DIR_LIST *tmp_ptr;
        char *nm_ptr = strtok(path, "/");
        printf("%s",nm_ptr);
        while (nm_ptr != NULL)//�̸��� NULL���� �ƴ϶��
        {
            if (first == 0)//���۵��丮�� . , .. , /�� �� �ϳ�  . �� ..���� �����Ҷ��� ��� �Ǵ�
            {
                if (nm_ptr == ".") //.���� ����
                {
                    tmp_ptr = rear_dir_list_ptr;
                    printf("%s\n", nm_ptr);
                    nm_ptr = strtok(NULL, "/");
                    first++;
                    continue;
                }
                else if (nm_ptr == "..")//..���� ����
                {
                    int i = 0;
                    tmp_ptr = front_dir_list_ptr;
                    while (i < cntfound()-1)
                    {
                        tmp_ptr = tmp_ptr -> next_ptr;
                    } // tmp_ptr�� ���� ���͸� ����Ʈ���� (������-1)��° ���丮�� ����Ű���� ��.
                    
                    tmp_ptr = rear_dir_list_ptr;
                    nm_ptr = strtok(NULL, "/");
                    first++;
                    continue;
                }
                else
                {
                    ;
                }
            }
            else if (first != 0)
            {
                DIR_LIST* new_dir = malloc(sizeof(DIR_LIST)); 
                new_dir-> name = nm_ptr;
                printf("%s",nm_ptr);
                char* s = strcat("/",nm_ptr);//s = /'���� ���丮'
                new_dir-> inode = path_to_inode(strcat(prtpwd(),s));
                tmp_ptr -> next_ptr = new_dir;
                nm_ptr = strtok(NULL, "/");
            }
        }
    }
    return;
}

/*
�̸�    : mycpto �Լ�
�ۼ���  : ���Ժ�
���    : MY ���Ͻý��ۿ� �ִ� ������ ȣ��Ʈ ��ǻ�ͷ� �����Ѵ�.
�޴°�  : ���ϸ� 1,2
���ϰ�  : ���ϰ�
*/
void mycpto (const char* source_file, const char* dest_file  )
{
    FILE *ofp;
    FILE *myfs;
    int c,d;
    int i = 0;
    int inode = 1;
    int tmp_datablock;

    if(source_file ==NULL || dest_file == NULL)
    {
        printf("���� : ���ڰ� �����մϴ�");
        return;
    }
    char *tmp_file_string_ptr = (char *)malloc(sizeof(char) * 8); //���丮�� datablock���� ������ ���ϸ��� ����ų ������
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //���丮�� datablock���� ������ inode ��ȣ�� ����ų ������
    INODE *inode_ptr = (INODE *)malloc(sizeof(INODE));// inode ������

    myfs = fopen("myfs", "rb");
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (rear_dir_list_ptr -> inode - 1)), SEEK_SET);
    fread(inode_ptr, sizeof(INODE), 1, myfs);

    tmp_datablock = (int)(inode_ptr->dir_1 + 1);
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+ INODE_LIST_SIZE +  (256 * (tmp_datablock - 1)),SEEK_SET);//���� ���丮�� �����ͺ�� ������ ������ �̵�
    fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs); // �����ͷ� ���ϸ� Ȯ��

    while(!strcmp(tmp_file_string_ptr, source_file))
    {//ã�� ���ϰ� Ȯ���� ������ �̸��� ���� �ʴٸ�
        if((c = getchar()) != EOF)
        {//myfs������ �� Ȯ��
            fseek(myfs, -1, SEEK_CUR);
            fseek(myfs,sizeof(int),SEEK_CUR);//���� ������ ��ġ�κ��� int�� ũ�⸸ŭ �̵�
            fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs);
        }
        else
        {
        printf("���� : %s ������ �����ϴ�.\n", source_file); //���ϸ��� ���� ���丮���� ��ã���� ���� ����
        return;
        }
    }

    fread(tmp_inode_ptr, sizeof(int), 1, myfs);
    inode =*tmp_inode_ptr;

    if ((ofp = fopen(dest_file, "wb")) == NULL)
    {
        printf("���� : %s ������ �� �� �����ϴ�. \n", dest_file);
        return;
    }

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (inode - 1)), SEEK_SET);
    fread(inode_ptr, sizeof(INODE), 1, myfs);

    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+INODE_LIST_SIZE +(DATA_BLOCK_SIZE*((inode_ptr->dir_1)- 1)),SEEK_SET);//���ο� ���Ͽ� ����
    while(i != DATA_BLOCK_SIZE)
    {
        d = getc(myfs);
        putc(d,ofp);
        i++;
    }
    
    free(tmp_file_string_ptr);
    free(tmp_inode_ptr);
    free(inode_ptr);
    fclose(ofp);
    fclose(myfs);

    return;
}

/*
�̸�    : mycpfrom �Լ�
�ۼ���  : ���Ժ�
���    : ȣ��Ʈ ��ǻ�Ϳ� �ִ� ������ MY ���Ͻý������� �����Ѵ�.
�޴°�  : ���ϸ� 1,2
���ϰ�  : ���ϰ�
*/

void mycpfrom (const char* source_file, const char* dest_file )
{
    FILE *ifp;
    FILE *myfs;
    int c,size_F;
    time_t Time;
    struct tm* TimeInfo;
 
    Time = time(NULL);                // ���� �ð��� ����
    TimeInfo = localtime(&Time); 

    myfs = fopen("myfs", "rb+");
    char *tmp_dir_string_ptr = (char *)malloc(sizeof(char) * 8); //���丮�� datablock���� ������ ���丮���� ����ų ������
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //���丮�� datablock���� ������ inode ��ȣ�� ����ų ������
    INODE *inode_data_ptr = (INODE *)malloc(sizeof(INODE));
    
    int new_inode = acc_inode();
    int new_data = acc_data();
    ifp = fopen(source_file,"rb");
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*(new_data-1)),SEEK_SET);//���ο� ���Ͽ� ����
    while ((c = getc(ifp)) != EOF)
    {
      size_F++;
      putchar(c);
    }

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (new_inode - 1)),SEEK_SET);//INODELIST ä���
    fread(inode_data_ptr, sizeof(INODE), 1, myfs);
    inode_data_ptr -> type = 0;
    inode_data_ptr -> year = TimeInfo ->tm_year+1900;
    inode_data_ptr-> size = size_F;
    inode_data_ptr -> month = TimeInfo ->tm_mon+1;
    inode_data_ptr -> date = TimeInfo ->tm_mday;
    inode_data_ptr -> hour = TimeInfo ->tm_hour;
    inode_data_ptr -> minute = TimeInfo ->tm_min;
    inode_data_ptr -> second = TimeInfo ->tm_sec;
    inode_data_ptr -> dir_1 = new_data;    
    inode_data_ptr -> dir_2 = 0;
    inode_data_ptr -> dir_3 = 0;
    inode_data_ptr -> dir_4 = 0;
    inode_data_ptr -> dir_5 = 0;
    inode_data_ptr -> dir_6 = 0;
    inode_data_ptr -> dir_7 = 0;
    inode_data_ptr -> dir_8 = 0;
    inode_data_ptr -> indir = 0; 
    fwrite(inode_data_ptr,sizeof(INODE),1,myfs);

    

    fclose(ifp);
    fclose(myfs);

    return;
}

/*
�̸�    : mycp �Լ�
�ۼ���  : ���Ժ�
���    : MY ���Ͻý��ۿ� �ִ� ������ �����ϴ� ��ɾ�
�޴°�  : ���ϸ� 1,2
���ϰ�  : ���ϰ�
*/

void mycp(const char* source_file, const char* dest_file  )
{
    FILE *myfs;
    int c,size_F;;
    int i = 0;
    int inode = 1;
    char tmp;
    char* d;
    time_t Time;
    struct tm* TimeInfo;
 
    Time = time(NULL);                // ���� �ð��� ����
    TimeInfo = localtime(&Time); 

    char *tmp_file_string_ptr; //���丮�� datablock���� ������ ���ϸ��� ����ų ������
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //���丮�� datablock���� ������ inode ��ȣ�� ����ų ������
    INODE *inode_data_ptr = (INODE *)malloc(sizeof(INODE));// inode ������
    char *tmp_data_string; //���丮�� datablock���� ������ �����͸� ����ų ������

    myfs = fopen("myfs", "rb+");
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*(path_to_inode(prtpwd())-1)),SEEK_SET);//���� ���丮�� �����ͺ�� ������ ������ �̵�
    fread(inode_data_ptr, sizeof(INODE), 1, myfs);
    int tmp_datablock = (int)(inode_data_ptr->dir_1 + 1);
    fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs); // �����ͷ� ���ϸ� Ȯ��

    while(!strcmp(tmp_file_string_ptr, source_file))
    {//ã�� ���ϰ� Ȯ���� ������ �̸��� ���� �ʴٸ�
        if((c = getchar()) != EOF)
        {//myfs������ �� Ȯ��
            fseek(myfs, -1, SEEK_CUR);
            fseek(myfs,sizeof(int),SEEK_CUR);//���� ������ ��ġ�κ��� int�� ũ�⸸ŭ �̵�
            fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs);
        }
        else
        {
        fprintf(stderr,"���� : %s ������ �����ϴ�.\n", source_file); //���ϸ��� ���� ���丮���� ��ã���� ���� ����
        exit(1);
        }
    }
    fread(tmp_inode_ptr, sizeof(int), 1, myfs);
    inode =*tmp_inode_ptr;

    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*((inode_data_ptr->dir_1)- 1)),SEEK_SET);//���ڿ� �����Ϳ� �����س���
    while(i != DATA_BLOCK_SIZE)
    {
        tmp = getchar();
        sprintf(d,"%d",tmp);
        strcat(tmp_data_string,d);
        i++;
    }

    fclose(myfs);

    int new_inode = acc_inode();
    int new_data = acc_data();
    
    rewind(myfs);
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*(new_data-1)),SEEK_SET);//���ο� ���Ͽ� ����
    int k = 0;
    int j = sizeof(tmp_data_string);
    while (k < j);
    {
      size_F++;
      putchar(c);
      k++;
    }

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (new_inode - 1)),SEEK_SET);//INODELIST ä���
    fread(inode_data_ptr, sizeof(INODE), 1, myfs);
    inode_data_ptr -> type = 0;
    inode_data_ptr -> year = TimeInfo ->tm_year+1900;
    inode_data_ptr-> size = size_F;
    inode_data_ptr -> month = TimeInfo ->tm_mon+1;
    inode_data_ptr -> date = TimeInfo ->tm_mday;
    inode_data_ptr -> hour = TimeInfo ->tm_hour;
    inode_data_ptr -> minute = TimeInfo ->tm_min;
    inode_data_ptr -> second = TimeInfo ->tm_sec;
    inode_data_ptr -> dir_1 = new_data;    
    inode_data_ptr -> dir_2 = 0;
    inode_data_ptr -> dir_3 = 0;
    inode_data_ptr -> dir_4 = 0;
    inode_data_ptr -> dir_5 = 0;
    inode_data_ptr -> dir_6 = 0;
    inode_data_ptr -> dir_7 = 0;
    inode_data_ptr -> dir_8 = 0;
    inode_data_ptr -> indir = 0; 
    fwrite(inode_data_ptr,sizeof(INODE),1,myfs);

    
    free(tmp_file_string_ptr);
    free(tmp_inode_ptr);
    fclose(myfs);

    return;
}

/*
�̸�    : myrm �Լ�
�ۼ���  : ���Ժ�
���    : ������ �����ϴ� ��ɾ�
�޴°�  : ���ϸ�
���ϰ�  : ���ϰ�
*/

void myrm(const char* file)
{
    FILE *myfs;
    int c,i=0;
    int inode = 1;

    char *tmp_file_string_ptr = (char *)malloc(sizeof(char) * 8); //���丮�� datablock���� ������ ���ϸ��� ����ų ������
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //���丮�� datablock���� ������ inode ��ȣ�� ����ų ������
    INODE *inode_ptr = (INODE *)malloc(sizeof(INODE));// inode ������

    myfs = fopen("myfs", "rb+");
    if (myfs == NULL)
    {
        printf("myrm() �Լ� : ���� ���⿡ �����߽��ϴ�.\n");
        abort();
    }
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*((path_to_inode(prtpwd()))-1)),SEEK_SET);//���� ���丮�� �����ͺ�� ������ ������ �̵�
    fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs); // �����ͷ� ���ϸ� Ȯ��

    while(!strcmp(tmp_file_string_ptr, file))
    {//ã�� ���ϰ� Ȯ���� ������ �̸��� ���� �ʴٸ�
        if((c = getchar()) != EOF)
        {//myfs������ �� Ȯ��
            fseek(myfs, -1, SEEK_CUR);
            fseek(myfs,sizeof(int),SEEK_CUR);//���� ������ ��ġ�κ��� int�� ũ�⸸ŭ �̵�
            fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs);
        }
        else
        {
        fprintf(stderr,"���� : %s ������ �����ϴ�.\n", file); //���ϸ��� ���� ���丮���� ��ã���� ���� ����
        exit(1);
        }
    }
    fread(tmp_inode_ptr, sizeof(int), 1, myfs);
    inode =*tmp_inode_ptr;
    fclose(myfs);

    rewind(myfs);
    fseek(myfs, BOOT_BLOCK_SIZE + (inode- 1) ,SEEK_SET);//INODELIST 
    putchar(0);
    fseek(myfs, BOOT_BLOCK_SIZE + 128 + ((inode_ptr -> dir_1) - 1),SEEK_SET);//DATABLOCK
    putchar(0);
    fseek(myfs, BOOT_BLOCK_SIZE + (sizeof(INODE)*128)+(DATA_BLOCK_SIZE)*((inode_ptr -> dir_1) - 1) ,SEEK_SET);
    while(i != DATA_BLOCK_SIZE)
    {
        putchar(0);
        i++;
    }
}

/*
�̸�    : cntfound �Լ�
�ۼ���  : ���Ժ�
���    : dir_list���� ���� �̾��� ���丮 �� ����
�޴°�  : X
���ϰ�  : count��
*/

int cntfound() 
{
    int cnt = 0;
    DIR_LIST* tmp_dir = front_dir_list_ptr;
    while ((tmp_dir->next_ptr) != NULL)
    {
        {
            tmp_dir = tmp_dir->next_ptr;
            cnt++;
            printf("fff\n");
        }
    }
    return cnt;//Ž�� ���н�

}

/*
�̸�    : prtpwd �Լ�
�ۼ���  : ���Ժ�
���    : pwd�� ���ڿ��� ����
�޴°�  : X
���ϰ�  : pwd ���ڿ���
*/
char* prtpwd()
{
    char* s;
    char* pwd;
    DIR_LIST *tmp_ptr = front_dir_list_ptr;

    while (1)
    {
        s = tmp_ptr->name;
        pwd = strcat(pwd,s);
        if (tmp_ptr->next_ptr == NULL)
        {
            break;
        }
        else
        {
            pwd = strcat(pwd,"/");
            tmp_ptr = tmp_ptr->next_ptr;
        }
    }
    return pwd;
}

/*
�̸�    : acc_inode �Լ�
�ۼ���  : ���Ժ�
���    : ���������� ���� inode ��ȣ�� ��������, �� inode��ȣ�� �ش��ϴ� ���ۺ���� 1�� ä���.
�޴°�  : X
���ϰ�  : ���������� ���� inode ��ȣ
*/

int acc_inode()
{
    FILE* myfs;
    int t = 1;

    myfs = fopen("myfs", "rb");
    if (myfs == NULL)
    {
        printf("acc_inode() �Լ� : ���� ���⿡ �����߽��ϴ�.\n");
        abort();
    }

    SUPERBLOCK *sb_ptr = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
    fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
    fread(sb_ptr, sizeof(SUPERBLOCK), 1, myfs);//������� ���ۺ�� Ȯ���Ͽ� ���������� ���� ���̳�忡 ���� �ֱ� ����.

    unsigned mask;
    while(1){ // �� inode�� ������ ���� ���� �ݺ�
    if (t < 33) // inode_1�� �� inode�� ���� ���
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->inode_1 & mask) == 0) //�� ���̳�尡 �־��� ���
        {
            sb_ptr->inode_1 = sb_ptr->inode_1 | 1;//�� ���̳�� ä���
            break;
        }
        t++;
    }
    else if (t < 65) // inode_2�� �� inode�� ���� ���
    {
        mask = 1 << ((t- 32) - 1);
        if ((sb_ptr->inode_2 & mask) == 0)
        {
            sb_ptr->inode_2 = sb_ptr->inode_2 | 1;
            break;
        }
        t++;
    }
    else if (t < 97) // inode_3�� �� inode�� ���� ���
    {
        mask = 1 << ((t - 64) - 1);
        if ((sb_ptr->inode_3 & mask) == 0)
        {
          sb_ptr->inode_3 = sb_ptr->inode_3 | 1;
          break;
        }
        t++;
    }
    else // inode_4�� �� inode�� ���� ���
    {
        mask = 1 << ((t - 96) - 1);
        if ((sb_ptr->inode_4 & mask) == 0)
        {
            sb_ptr->inode_4 = sb_ptr->inode_4 | 1;
            break;
        }
        t++;
    }
    }
    return t;
}

/*
�̸�    : acc_data �Լ�
�ۼ���  : ���Ժ�
���    : ���������� ���� datablock ��ȣ�� ��������, �� inode��ȣ�� �ش��ϴ� ���ۺ���� 1�� ä���.
�޴°�  : X
���ϰ�  : ���������� ���� datablock ��ȣ
*/
int acc_data()
{
    FILE* myfs;
    int t = 1;

    myfs = fopen("myfs", "rb");
    if (myfs == NULL)
    {
        printf("acc_data() �Լ� : ���� ���⿡ �����߽��ϴ�.\n");
        abort();
    }

    SUPERBLOCK *sb_ptr = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
    fseek(myfs, BOOT_BLOCK_SIZE+128, SEEK_SET);
    fread(sb_ptr, sizeof(SUPERBLOCK), 1, myfs);//������� ���ۺ�� Ȯ���Ͽ� ���������� ���� �����ͺ�Ͽ� ���� �ֱ� ����.

    unsigned mask;
    while(1)
    { // �� �����ͺ���� ������ ���� ���� �ݺ�
    if (t < 33) // data_block_1�� �� data_block�� ���� ���
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_1 & mask) == 0) //�� ���ۺ���� �־��� ���
        {
            sb_ptr->data_block_1 = sb_ptr->data_block_1 | 1;//�� ���ۺ�� ä���
            break;
        }
        t++;
    }
    else if (t < 65) // data_block_2�� �� data_block�� ���� ���
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_2 & mask) == 0) //�� ���ۺ���� �־��� ���
        {
            sb_ptr->data_block_2 = sb_ptr->data_block_2 | 1;//�� ���ۺ�� ä���
            break;
        }
        t++;
    }
    else if (t < 97) // data_block_3�� �� data_block�� ���� ���
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_3 & mask) == 0) //�� ���ۺ���� �־��� ���
        {
            sb_ptr->data_block_3 = sb_ptr->data_block_3 | 1;//�� ���ۺ�� ä���
            break;
        }
        t++;
    }
    else if (t < 129) // data_block_4�� �� data_block�� ���� ���
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_4 & mask) == 0) //�� ���ۺ���� �־��� ���
        {
            sb_ptr->data_block_4 = sb_ptr->data_block_4 | 1;//�� ���ۺ�� ä���
            break;
        }
        t++;
    }
    else if (t < 161) // data_block_5�� �� data_block�� ���� ���
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_5 & mask) == 0) //�� ���ۺ���� �־��� ���
        {
            sb_ptr->data_block_5 = sb_ptr->data_block_5 | 1;//�� ���ۺ�� ä���
            break;
        }
        t++;
    }
    else if (t < 193) // data_block_6�� �� data_block�� ���� ���
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_6 & mask) == 0) //�� ���ۺ���� �־��� ���
        {
            sb_ptr->data_block_6 = sb_ptr->data_block_6 | 1;//�� ���ۺ�� ä���
            break;
        }
        t++;
    }
    else if (t < 225) // data_block_7�� �� data_block�� ���� ���
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_7 & mask) == 0) //�� ���ۺ���� �־��� ���
        {
            sb_ptr->data_block_7 = sb_ptr->data_block_7 | 1;//�� ���ۺ�� ä���
            break;
        }
        t++;
    }
    else // data_block_3�� �� data_block�� ���� ���
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_8 & mask) == 0) //�� ���ۺ���� �־��� ���
        {
            sb_ptr->data_block_8 = sb_ptr->data_block_8 | 1;//�� ���ۺ�� ä���
            break;
        }
        t++;
    }
    }
    return t;
}
