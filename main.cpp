#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define MAX_queue 100

/*結構宣告----------------------------------------*/
struct tree {
    int No;                //節點編號
    int key;               //節點存的值
    struct tree *back;     //回到父節點的指標
    struct tree *left;     //往左邊跑的指標
    struct tree *right;    //往右邊跑的指標
};

/*全域變數宣告------------------------------------*/
int rear=-1, front=-1, total_high=0;	//控制佇列的移動
struct tree *queue[MAX_queue];    		//實現佇列的陣列

/*函式宣告---------------------------------------*/
void add(struct tree *root);         //add函式
void addq(struct tree *item);        //add用函式
struct tree* deleteq();              //pop函式
void display(struct tree *root);     //印出樹的函式
void search(struct tree *root);      //搜尋
void delete_node(struct tree **root);//刪除
void change_NO(struct tree *root);   //刪除用
void inorder(struct tree *root);     //inorder函式
void represent(struct tree *root);

int main() {
    int choose, high=0;
    struct tree *root;
    /*root construct*/
    root=(struct tree*)malloc(sizeof(struct tree));
    memset(root, 0, sizeof(struct tree));
    printf("Please input a value:");
    scanf("%d", &root->key);
    root->No=1;
    printf("Your root: %d is constructed\n",root->key);

    /*menu*/
    while(1) {
        printf("\n<1> Display <2> Add <3> Delete <4> Search <5> Inorder <6> Exit:");
        scanf("%d", &choose);
        switch(choose) {
            case 1:					//秀出樹狀圖
                display(root);
                printf("\n\n");
                printf("(");
                represent(root);
                printf(")");
                break;
            case 2:					//新增節點
                add(root);
                break;
            case 3:					//刪除節點
                delete_node(&root);
                break;
            case 4:					//搜尋節點
                search(root);
                break;
            case 5:					//排序
                inorder(root);
                break;
            case 6:					//結束
                printf("Exit\n");
                exit(1);
                break;
            default:				//輸入錯誤
                printf("Input ERROR!!\n");
                break;
        }
    }
    return 0;
}

void add(struct tree *root)
{
    struct tree *child;
    int add_ok = 0, high = 0, up_No;
    /*子節點建構*/
    child =(struct tree *)malloc(sizeof(struct tree));
    memset(child, 0, sizeof(struct tree));
    printf("Please input a value:");
    scanf("%d",&child->key);

    /*選擇安插位置*/
    do
    {
        if(child->key < root->key)//子節點的值比父節點小
        {
            if(root->left==NULL)//如果父節點的左邊有空位則插入
            {
                root->left = child;
                child->No = 2 * root->No;
                child->back = root;
                add_ok = 1;//增加完畢
            }
            root = root->left; //沒有空位(有其他節點已存在) 則繼續往下跑
            high++;
        }
        else if(child->key > root->key)//子節點的值比父節點大
        {
            if(root->right==NULL)//父節點右邊有空位則插入
            {
                root->right = child;
                child -> No = 2 * root->No + 1;
                child->back = root;
                add_ok = 1;//增加完畢
            }
            root=root->right;//有其他節點存在 繼續往下跑
            high++;
        }
        else//輸入的值已存在樹裡
        {
            printf("Error! This value has existed!!\n");
            break;
        }
    }while(add_ok!=1);
    if(high > total_high)//判斷目前樹有多高
        total_high = high;
}

void addq(struct tree *item) {
    queue[++rear]=item;
}

struct tree* deleteq() {
    return queue[++front];
}

void display(struct tree *root)//印出樹型
{
    int final[MAX_queue],i=0,j=0,power,run,run_total_1,run_total_2,high_1,high_2;
    rear=-1;
    front=-1;
    memset(final,-1,sizeof(int)*MAX_queue);
    /*先做level order traversal,並把值存入陣列*/
    addq(root);
    final[1]=root->key;
    for(;;)
    {
        root=deleteq();
        if(root)
        {
            if(root->left)
            {
                addq(root->left);
                final[(root->left)->No] = (root->left)->key;
            }

            if(root->right)
            {
                addq(root->right);
                final[(root->right)->No] = (root->right)->key;
            }
        }
        else
            break;
    }
    run=pow(2,total_high+1);//計算要跑多少次
    high_1=total_high;
    high_2=total_high+1;

    power=1;
    for(i=1;i<=run-1;i++)
    {
        if(i==power)
        {
            printf("\n\n");
            run_total_1 = pow(2,high_1);//計算要印出幾個空格
            for(j = 0 ; j < (run_total_1 - 1) * 4 ; j++)
                printf(" ");
            if(final[i]== -1)
                printf("    ");
            else
                printf("(%2d)",final[i]);
            high_1--;
            if(i!=1)
                high_2--;
            power = power * 2; //控制什麼時候換行
        }

        else
        {
            run_total_2 = pow(2,high_2);
            for(j = 0 ; j < (run_total_2 - 1) * 4 ; j++)//控制節點間的寬度
                printf(" ");
            if(final[i]== -1)
                printf("    ");
            else
                printf("(%2d)",final[i]);
        }
    }
}

void inorder(struct tree* root)//inorder
{
    if(root!=NULL)
    {
        inorder(root->left);//先往左邊跑
        printf(" %d",root->key);//印出值
        inorder(root->right);//再往右邊跑
    }
}

void search(struct tree *root)//搜尋函式
{
    int get = 0,want_to_search;
    printf("Whitch value do you want to search: ");//輸入想要找的值
    scanf("%d",&want_to_search);
    while(root)
    {
        if(root->key==want_to_search)//如果存的值跟想要的一樣 get=1
        {
            get = 1;
            break;
        }
        else if(want_to_search < root->key)//如果想找的值比節點的小
            root = root->left;//往左邊跑
        else if(want_to_search > root->key)//想找的值比節點大
            root = root->right;//往右邊跑
    };
    if(get==1)//找到
        printf("The value you want to search:%d in tree\n",want_to_search);
    else//沒找到
        printf("The value you want to search:%d NOT in tree\n",want_to_search);
}

void delete_node(struct tree **root)
{
    int want_to_del,get,temp_1,temp_2;
    struct tree *temp,*del;
    printf("\nInput the value you want to delete:");
    scanf("%d",&want_to_del);
    /*search*/
    del = *root;
    while(del)//先做搜尋(方法跟search一樣)
    {
        if(del->key==want_to_del)
        {
            get = 1;
            break;
        }
        else if(want_to_del < del->key)
            del = del->left;
        else if(want_to_del > del->key)
            del = del->right;
    };
    if(get==1)//如果該值存在樹裡
    {
        if(del->left==NULL&&del->right==NULL)//末端
        {
            if(del==*root)//第一個節點
            {
                printf("\nThe old root:%d,has been deleted!\n",del->key);
                printf("Please input a new root:");
                scanf("%d",&(*root)->key);
                printf("Your new root: %d,has been constructed",(*root)->key);
            }
            else
            {
                temp = del->back;//temp指著父節點
                //printf("temp->right->key:%d",temp->right->key);
                if(temp->left!=NULL&&(temp->left)->key==del->key)//如果要刪除的值在temp左邊
                    temp->left = NULL;
                else if(temp->right!=NULL&&(temp->right)->key==del->key)//要刪的值在temp右邊
                    temp->right = NULL;
                free(del);
            }
        }
        else if(del->left!=NULL&&del->right!=NULL)//健全的root
        {
            temp = del;
            temp = temp->left;//雙邊先踏左路
            if(temp->left==NULL&&temp->right==NULL)//末端
            {
                del->key = temp->key;
                del->left = NULL;
                free(temp);
            }
            else if(temp->right==NULL)//單邊有樹
            {
                change_NO(temp);//編號更換(單邊樹一定有用到這個函式)
                temp->right = del->right;
                (del->right)->back = temp;
                free(del);
            }
            else if(temp->right!=NULL)//雙邊在走右路
            {
                while(temp->right!=NULL)
                    temp = temp->right;
                del->key = temp->key;//走到底把底層的數值丟給root
                if(temp->left==NULL)
                    (temp->back)->right = NULL;
                else//如果被change的節點左邊還有值
                {
                    (temp->back)->right=temp->left;
                    change_NO(temp->left);
                }
                free(temp);
            }
        }
        else//殘廢的root
        {
            if(del->left==NULL)//斷左邊
            {
                if(del==*root)//第一個節點
                {
                    (*root) = (*root)->right;
                    change_NO(*root);//編號更改
                }
                else
                {
                    temp = del;
                    temp = temp->back;
                    if(temp->left!=NULL&&temp->left->key == del->key)
                    {
                        del->right->back = temp;
                        temp->left = del->right;//把子樹往上拉
                        free(del);
                        change_NO(temp->left);//編號更改
                    }
                    else if(temp->right->key == del->key)
                    {
                        del->right->back = temp;
                        temp->right = del->right;//把子樹往上拉
                        free(del);
                        change_NO(temp->right);//編號更改
                    }
                }
            }
            else if(del->right==NULL)//斷右邊
            {
                if(del==*root)//第一個節點
                {
                    (*root) = (*root)->left;
                    change_NO(*root);
                }
                else
                {
                    temp = del;
                    temp = temp->back;
                    if(temp->left!=NULL&&temp->left->key == del->key) {
                        del->left->back = temp;
                        temp->left = del->left;//把樹往上拉
                        free(del);
                        change_NO(temp->left);//編號更改
                    }
                    else if(temp->right->key == del->key) {
                        del->left->back = temp;
                        temp->right = del->left;//把樹往上拉
                        free(del);
                        change_NO(temp->right);//編號更改
                    }
                }
            }
        }
    }
    else
        printf("Sorry!!The value you want to delete:%d NOT in tree\n",want_to_del);//刪除的值不在樹內
}

void change_NO(struct tree *root)//編號改變函式 會將丟入的樹的編號除以2(往上拉)
{
    if(root!=NULL) {
        change_NO(root->left);
        root->No = (root->No)/2;
        change_NO(root->right);
    }
}

void represent(struct tree* root)//inorder
{
    if(root!=NULL) {
        printf(" %d",root->key);//印出值
        if(root->left)//左邊有樹
        {
            printf("(");
            represent(root->left);
        }
        else           //只有右邊有樹
        {
            if(root->right)
                printf("(");
        }
        if(root->right)  //右邊有樹
        {
            if(root->left)
                printf(",");
            represent(root->right);
            printf(")");
        }
        else              //只有左邊有樹
        if(root->left)
            printf(")");
    }
}
