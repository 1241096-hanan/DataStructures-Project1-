
//Hanan Mohammad Hassan Hamad-1241096-Section(3)


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 250

//Linked list to store all equations from file
struct node {
    char equation[MAX_LEN];
    struct node* next;
};

typedef struct node* List;
typedef List Position;


//stack for chars, used for brackets and convert infix to postfix
struct charNode{
    char data;
    struct charNode* next;
};
typedef struct charNode* stackChar;

//stack for numbers, used to evalute postfix
struct nodeNum {
    int element;
    struct nodeNum* next;
};
typedef struct nodeNum* StackNum;

//tree node for expression tree
struct Tree {
    char data[70];
    struct Tree* left;
    struct Tree* right;
};
typedef struct Tree* TreeNode;

//stack for tre nodes, used to build the tree
struct StackNode {
    TreeNode element;
    struct StackNode* next;
};
typedef struct StackNode* StackTree;


//Functions Prototypes
List MakeEmptyExp(List L);
void Insert(char eq[], List L);
void DeletList(List L);
Position GetEquation(List L, int idx);
void cleanList(List L);

int readFile(List L, char fName[]);
void wOutputFile(List L);

int isCharStackEmpty(stackChar S);
stackChar createCharStack();
void pushChar(stackChar S, char x);
void popChar(stackChar S);
char topChar(stackChar S);
void MakeEmptyChar(stackChar S );
void DisposeChar(stackChar S);


StackNum CreateStackNum();
int IsEmptyNum(StackNum S);
void PushNum(int x, StackNum S);
int TopNum(StackNum S);
void PopNum(StackNum S);
void DisposeNum(StackNum S);
void MakeEmptyNum(StackNum S );

TreeNode CreateTreeNode(char x[]);
void freeTree(TreeNode root);
StackTree CreateStackTree();
int IsEmptyTree(StackTree S);
void PushTree(TreeNode x, StackTree S);
TreeNode TopTree(StackTree S);
void PopTree(StackTree S);


int IsOp(char op);
int IsMatching(char open, char close);
int getPriority(char op);
void removeSpace(char eq[], char space[]);
int IsValid(char eq[], char msg[]);
void infixToPost(char eq[], char post[]);
int evaluatePostfix(char post[]);

TreeNode BuildTree(char post[]);

void inorder(TreeNode root);
void preorder(TreeNode root);
void postorder(TreeNode root);



int main() {

    List exp;
    exp=MakeEmptyExp(NULL);// initialize list

    int count=0;// to count total equations
    int load=0; // to check if user loaded the file

    char post[MAX_LEN];
    char errorMsg[200];
    char eqClean[MAX_LEN];// to store equation without space

    int option;
    printf("\n(Expression Processing System)");

    do {
        // Menu
        printf("\n\t---- MENU ----\n");

        printf("1. Read expressions from file\n");
        printf("2. Check the validity of equations\n");
        printf("3. Convert valid equation from infix to Postfix\n");
        printf("4. Evaluate postfix expressions and print results\n");
        printf("5. Show invalid equations\n");
        printf("6. Expression Tree options\n");
        printf("7. Save results to output.txt\n");
        printf("8. Exit\n");

        printf("\n Enter your Choice: ");
        if (scanf("%d", &option) != 1) {// check if the user enter letter insted of number
            printf("Invalid input!\nPlease enter valid number(1-8)\n");
            while (getchar()!= '\n');//clear buffer
            continue;
         }
          // to check if the user loaded the equation from file or not
          if(option >= 2 && option <= 7 && !load){
           char fName[100];
           printf("\nYou need to load a file first.\n");
           printf("Enter file name(e.g. file.txt): ");
           scanf("%s", fName);
           count=readFile(exp, fName);

          if(count>0) {
             load=1;
             printf("File loaded successfully.\nTotal equations = %d\n\n", count);
          }
        else{
             printf("No file loaded, you need to load a file first.\n");
             continue;
         }
      }

        switch(option) {

        case 1: {
              if (load == 1) {
                 printf("File already loaded.\n");
                 break;
             }

           char fName[100];
            printf("Enter file name(e.g. file.txt): ");
             scanf("%s", fName);
            count = readFile(exp, fName);
         if (count > 0) {
             load = 1;
           printf("Successfully loaded %d expressions.\n", count);
      } else {
           printf("Error loading file.\n");
        }

       break;
       }

        case 2:{
                Position p=exp->next;
                int i=1;
                // loop through all equations and check validity
                while (p != NULL) {
                  if(IsValid(p->equation, errorMsg))
                     printf("Equation No. %d -> Valid\n", i);
                  else
                    printf("Equation No. %d -> %s\n", i, errorMsg);

                 p=p->next;
                 i++;
               }
                break;
            }

        case 3:{
                 Position p=exp->next;
                 int i = 1;
                 // convert valid eq to postfix
                 while(p != NULL) {
                   if(IsValid(p->equation, errorMsg)) {
                      removeSpace(p->equation, eqClean);
                      infixToPost(eqClean, post);
                      printf("Equation No. %d -> %s\n", i, post);
                }

                 p = p->next;
                 i++;
            }

            break;
        }
        case 4:{
              Position p=exp->next;
              int i=1;
             // evaluate and print the result for valid eq
             while (p!= NULL) {
                 if(IsValid(p->equation, errorMsg)) {
                    removeSpace(p->equation, eqClean);// Remove whitspace to ensure correct parsing of the expression

                    infixToPost(eqClean,post);

                    int result = evaluatePostfix(post);
                    printf("Equation No. %d -> Result = %d\n",i,result);
                }

                p=p->next;
                i++;
            }

            break;
        }


        case 5:{
            Position p=exp->next;
            int i=1;
            int checkInvalid = 0;
            // print only the invalid ones
            while(p!= NULL) {
                if (!IsValid(p->equation, errorMsg)) {
                    printf("Equation No. %d -> %s\n",i,p->equation);
                    checkInvalid = 1;
                }
                p = p->next;
                i++;
            }
                if (!checkInvalid)
                    printf("No invalid equations found.\n");
                break;
        }
        case 6: {
                int idx;
                printf("Enter equation index from (1 to %d): ", count);
                scanf("%d", &idx);
                if (idx < 1 || idx > count) {
                printf("Invalid index!\n");
                break;
            }

                Position p = GetEquation(exp, idx);
              if (p == NULL) {
                  printf("Equation not found!\n");
                  break;
            }
            // build and print expression tree traversals
            if (IsValid(p->equation, errorMsg)) {
                  removeSpace(p->equation, eqClean);
                  infixToPost(eqClean, post);

                 TreeNode root = BuildTree(post);

                 if(root!=NULL){
                 printf("\nInorder (Infix):\n");
                 inorder(root);

                 printf("\n\nPostorder (Postfix):\n");
                 postorder(root);

                 printf("\n\nPreorder (Prefix):\n");
                 preorder(root);

                 printf("\n");

                freeTree(root);}// free memory
            }

             else {
                   printf("Cannot build tree!\n");
                  }
                break;
            }

            case 7:
                 wOutputFile(exp);
                 break;

          case 8:
                printf("Goodbye!\n");
                break;

            default:
                printf("Invalid option, please try again.\n");
        }
    } while(option != 8);


    DeletList(exp);// free list before exit
    return 0;// end program
}




stackChar createCharStack() {

    stackChar S=(stackChar)malloc(sizeof(struct charNode));

    if (S == NULL){
        printf("Out of space!");
        return NULL;}

        S->next = NULL;
    return S;
}

void DeletList(List L) {

    Position p=L->next;
    Position temp;

    L->next=NULL;

    while (p!=NULL) {

        temp=p;
        p=p->next;

        free(temp);
    }
    free(L);
}


// use it to clear linked list
void cleanList(List L){

    Position p=L->next;
    Position temp;

    while (p!= NULL) {
        temp = p;
        p = p->next;
        free(temp);
    }
    L->next = NULL;
}

List MakeEmptyExp(List L) {

    if (L != NULL)
        DeletList(L);

     L=(List)malloc(sizeof(struct node));

    if (L == NULL)
        printf("Out of memory!\n");

    else
        L->next = NULL;

    return L;
}

void Insert(char eq[], List L) {

    Position temp;
    Position p=L;
    while (p->next != NULL)
        p=p->next;

    temp=(Position)malloc(sizeof(struct node));
    if (temp == NULL) {
        printf("Out of memory!\n");
        return;
    }
    strcpy(temp->equation, eq);
    temp->next=NULL;
    p->next=temp;
}

Position GetEquation(List L, int idx) {
    Position p=L->next;
    int k=1;
    while (p!=NULL && k< idx) {
        p=p->next;
        k++;
    }
    return p;
}

int isCharStackEmpty(stackChar S) {
    return (S->next == NULL);
}

void MakeEmptyChar(stackChar S){
  if( S == NULL )
     printf("Out of space!");
  else
   while(!isCharStackEmpty( S ))
     popChar(S);
 }

// free all nodes in the stack and delete the stack itself
void DisposeChar(stackChar S) {
    MakeEmptyChar(S);
    free(S);
}

void pushChar(stackChar S, char x) {

   stackChar newN=(stackChar)malloc(sizeof(struct charNode));
    if (newN == NULL)
        printf("Out of space");

    else {

        newN->data = x;
        newN->next = S->next;
        S->next = newN;
    }
}

void popChar(stackChar S) {

    if (isCharStackEmpty(S))
        printf("Empty Stack!");

    else {

        stackChar firstCell = S->next;

        S->next = firstCell->next;

        free(firstCell);
    }
}

char topChar(stackChar S) {

    if (!isCharStackEmpty(S))
        return S->next->data;

    printf("Empty Stack");

    return '\0';
}

StackNum CreateStackNum() {

    StackNum S=(StackNum)malloc(sizeof(struct nodeNum));

    if (S == NULL) {
        printf("Out of space!");
        return NULL;
    }

    S->next = NULL;
    return S;
}

int IsEmptyNum(StackNum S) {
    return (S->next == NULL);
}
void MakeEmptyNum(StackNum S){
  if( S == NULL )
     printf("Out of space!");
  else
    while(!IsEmptyNum( S ))
      PopNum(S);
 }

void DisposeNum(StackNum S) {
    MakeEmptyNum(S);
    free(S);
}

void PushNum(int x, StackNum S) {

    struct nodeNum* temp = (struct nodeNum*)malloc(sizeof(struct nodeNum));

    if (temp == NULL) {
        printf("Out of space!");
        return;
    }

    temp->element = x;
    temp->next = S->next;
    S->next = temp;
}

int TopNum(StackNum S) {

    if (!IsEmptyNum(S))
        return S->next->element;

    printf("Empty stack!\n");
    return 0;
}

void PopNum(StackNum S) {

    if (IsEmptyNum(S)) {
        printf("Empty stack\n");
        return;
    }

    StackNum temp = S->next;
    S->next = temp->next;
    free(temp);
}

StackTree CreateStackTree() {

    StackTree S =(StackTree)malloc(sizeof(struct StackNode));

    if (S == NULL) {
        printf("Out of space!");
        return NULL;
    }

    S->next = NULL;
    return S;
}

int IsEmptyTree(StackTree S) {
     return (S->next == NULL);
   }
void PushTree(TreeNode x, StackTree S) {

    struct StackNode* temp =(struct StackNode*)malloc(sizeof(struct StackNode));

    if (temp == NULL) {
        printf("Out of space!");
        return;
    }

    temp->element = x;
    temp->next = S->next;
    S->next = temp;
}

TreeNode TopTree(StackTree S) {

    if (!IsEmptyTree(S))
        return S->next->element;

    printf("Empty stack!\n");
    return NULL;
}

void PopTree(StackTree S) {

    if (IsEmptyTree(S)) {
        printf("Empty stack\n");
        return;
    }

    struct StackNode* temp = S->next;
    S->next = temp->next;
    free(temp);
}

TreeNode CreateTreeNode(char x[]) {

    TreeNode newNode=(TreeNode)malloc(sizeof(struct Tree));

    if (newNode==NULL) {
       printf("Out of memory!\n");
       return NULL;
    }

    strcpy(newNode->data, x);
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

void freeTree(TreeNode root) {
    if (root == NULL)
        return;

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int readFile(List L, char fName[]){

     FILE *file = fopen(fName, "r");

     if (file == NULL) {
        printf("File error\n");
        return 0;
     }
    cleanList(L);
    char line[MAX_LEN];
    int count=0;

    while (fgets(line, MAX_LEN, file) != NULL) {
           int len=strlen(line);

        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
               line[len - 1] = '\0';
               len--;
           }

     if(strlen(line) == 0)
        continue;

     Insert(line, L);
     count++;
  }
    fclose(file);
    return count;
}
// check if character is an operator
int IsOp(char op) {
    return(op=='+' ||op=='-' || op=='*' || op=='/'||op=='%' );
}
// check if brackets match each other
int IsMatching(char open, char close) {

    return(open=='(' && close==')') || (open=='[' && close==']')||
          (open=='{' && close=='}')||(open=='<' && close=='>') ;
}

// validation function to check brackets and operators errors
int IsValid(char eq[], char errorMsg[]) {

    stackChar S=createCharStack();
    char prev = '\0';

    for (int i=0; eq[i]!='\0'; i++) {

         char curr=eq[i];

         if (curr==' ' || curr=='\t')
             continue;

      // check for illegal characters
      if (!isdigit(curr) && !IsOp(curr) &&
            curr!='(' && curr!=')' && curr!='[' &&
            curr!=']' && curr!='{' && curr!='}'&&
            curr!='<' && curr!='>') {

            sprintf(errorMsg, "Invalid: character '%c' is not allowed", curr);
            DisposeChar(S);
            return 0;
         }

     if (curr == '-' && prev == '-') {
            sprintf(errorMsg, "Invalid: consecutive '-' operators (unsupported --5)");
            DisposeChar(S);
            return 0;
       }
      // can't start with a closing bracket
      if (prev=='\0' && (curr==')' || curr==']' || curr=='}' || curr=='>')) {
            sprintf(errorMsg,"Invalid: expression starts with closing bracket '%c'",curr);
            DisposeChar(S);
            return 0;
         }

      // push opening brackets to stack
      if (curr=='[' || curr=='{'||curr=='('||curr=='<'){
               pushChar(S,curr);
         }

      // handle closing brackets
      else if(curr==')' || curr==']' || curr=='}' || curr=='>'){
            if (isCharStackEmpty(S)){
                sprintf(errorMsg,"Invalid: closing bracket '%c' without opening bracket", curr);
                DisposeChar(S);
                return 0;
            }

            if (!IsMatching(topChar(S), curr)) {
                sprintf(errorMsg, "Invalid: mismatch between '%c' and '%c'",topChar(S), curr);
                DisposeChar(S);
                return 0;
            }
            if (IsOp(prev)) {
                sprintf(errorMsg, "Invalid: operator '%c' before closing bracket", prev);
                DisposeChar(S);
                return 0;
            }

            if ((curr == ')' && prev == '(') ||
                (curr == ']' && prev == '[') ||
                (curr == '}' && prev == '{') ||
                (curr == '>' && prev == '<')) {

                sprintf(errorMsg, "Invalid: empty brackets");
                DisposeChar(S);
                return 0;
            }
            popChar(S);
        }
        // handle operators rules
        else if (IsOp(curr)) {

            if (curr == '-' &&
               (prev == '\0' ||prev == '(' || prev == '[' ||
                prev == '{' || prev == '<' ||IsOp(prev))) {

               prev = curr;
               continue;}

            else {

             if (prev == '\0') {
             sprintf(errorMsg, "Invalid: expression starts with operator '%c'", curr);
             DisposeChar(S);
             return 0;
           }

            if (IsOp(prev)) {
            sprintf(errorMsg, "Invalid: two operators '%c' and '%c'", prev, curr);
            DisposeChar(S);
            return 0;
           }

            if (prev == '(' || prev == '[' || prev == '{' || prev == '<') {
            sprintf(errorMsg, "Invalid: operator '%c' after opening bracket", curr);
            DisposeChar(S);
            return 0;
           }
       }
 }

        // check for missing operators between numbers and brackets
        if (prev != '\0') {
            if ((isdigit(prev) || prev ==')' || prev==']' || prev== '}'|| prev== '>') &&
                (curr=='(' || curr=='[' || curr=='{'|| curr=='<')) {
                sprintf(errorMsg,"Invalid: missing operator between '%c' and '%c'", prev, curr);
                DisposeChar(S);
                return 0;
            }

            if ((prev == ')' || prev == ']' || prev == '}' || prev == '>')
                 &&isdigit(curr)) {

               sprintf(errorMsg,"Invalid: missing operator between '%c' and '%c'",prev, curr);
               DisposeChar(S);
               return 0;
           }

            if (isdigit(prev) && isdigit(curr) && eq[i - 1]==' ') {
                sprintf(errorMsg,"Invalid: missing operator between two number");
                DisposeChar(S);
                return 0;
            }
        }

        prev = curr;
    }
           // check if any bracket is still ope
          if (!isCharStackEmpty(S)) {
              sprintf(errorMsg, "Invalid: unclosed opening bracket '%c'", topChar(S));
              DisposeChar(S);
             return 0;
           }

          if (IsOp(prev)) {
              sprintf(errorMsg,"Invalid: ends with operator '%c'",prev);
              DisposeChar(S);
              return 0;
          }

    DisposeChar(S);
    return 1;
}
// operator precedence
int getPriority(char op) {

    switch (op) {
        case '*':
        case '/':
        case '%':
            return 2;

        case '+':
        case '-':
            return 1;

        default:
            return 0;
    }
}

// function to remove all spaces
void removeSpace(char eq[], char space[]) {
    int k=0;
    for (int i = 0; eq[i] != '\0'; i++) {
        if (eq[i] != ' ' && eq[i] != '\t') {
            space[k++] = eq[i];
        }
    }
    space[k] = '\0';
}

// convert infix string to postfix string using stack
void infixToPost(char eq[], char post[]) {

    stackChar s=createCharStack();
    int k=0;

  for (int i=0; eq[i]!='\0'; i++) {
       // if operand (number), add to output string
       if (isdigit(eq[i])) {
            while (isdigit(eq[i])) {
                post[k++] = eq[i++];
           }
             post[k++] = ' ';
             i--;
        }
      // handle negative numbers in postfix
      else if (eq[i] == '-' && (i == 0 || eq[i-1] == '(' || eq[i-1] == '[' || eq[i-1] == '{' || eq[i-1] == '<' || IsOp(eq[i-1]))) {
           if (eq[i+1] == '(') {
               // case: -(3+2)
               post[k++] = '0';
               post[k++] = ' ';
               pushChar(s, '-');
             }
          else {
              // case: -5
              post[k++] = '-';
               i++;
          while (isdigit(eq[i])) {
               post[k++] = eq[i++];
          }
            post[k++] = ' ';
            i--;
        }

     }
         // push opening brackets
         else if(eq[i]=='(' || eq[i]=='[' ||eq[i]=='{'||eq[i]=='<') {
            pushChar(s, eq[i]);
        }

        // pop until matching opening bracket is found
        else if (eq[i]==')' || eq[i]==']' || eq[i]=='}'||eq[i]=='>') {

            while (!isCharStackEmpty(s) && topChar(s)!='(' &&
                   topChar(s)!='[' && topChar(s)!='{'&&topChar(s)!='<') {

                post[k++] = topChar(s);
                post[k++] = ' ';
                popChar(s);
            }

            popChar(s);
        }

        // pop higher or equal priority operators then push current op
        else if (IsOp(eq[i])) {

            while (!isCharStackEmpty(s) &&
                   getPriority(topChar(s)) >= getPriority(eq[i])) {

                post[k++] = topChar(s);
                post[k++] = ' ';
                popChar(s);
            }

            pushChar(s, eq[i]);
        }
    }
    // pop all remaining operators from stack
    while (!isCharStackEmpty(s)) {

        post[k++] = topChar(s);
        post[k++] = ' ';
        popChar(s);
    }
    DisposeChar(s);
    post[k] = '\0';
}
// calculate the final result of postfix expression
int evaluatePostfix(char post[]) {

    StackNum s=CreateStackNum();
    int result, x, y;

    for (int i=0; post[i]!='\0'; i++) {

        if (post[i] == ' '|| post[i]=='\t')
            continue;

     // detrmine and store the sign of the number ->pos+|| neg-
     if (isdigit(post[i]) ||(post[i] == '-' && isdigit(post[i+1])))  {
           int sign = 1;
           if (post[i] == '-') {
              sign = -1;
              i++;
           }
      // to store full number when has more than one digit
           int n=0;
           while(isdigit(post[i])) {
             n=n*10+(post[i] - '0');
             i++;
        }
             n *= sign;
             i--;

          PushNum(n, s);
       }

    // if post[i] operator-> pop two numbers and apply operation
    else if(IsOp(post[i])) {
               if (IsEmptyNum(s)) {
                    printf("Error: invalid postfix expression\n");
                    DisposeNum(s);
                    return 0;
                }
                y=TopNum(s);
                PopNum(s);

            if (IsEmptyNum(s)) {
               printf("Error: invalid postfix expression\n");
                DisposeNum(s);
                return 0;
              }
              x=TopNum(s);
              PopNum(s);


           switch(post[i]) {
                case '+': result=x + y;
                          break;
                case '-': result=x-y;
                          break;
                case '*': result=x*y;
                          break;
                case '%':
                    if (y != 0) result= x%y;
                    else {
                        //printf("Error: Mod by zero!\n");
                        DisposeNum(s);
                        return(0);
                     }
                      break;

                case '/':
                    if (y != 0) result= x/y;
                    else {
                       // printf("Error: Division by zero!\n");
                        DisposeNum(s);
                        return(0);
                     }
                        break;

                default:
                    printf("Error: invalid operator '%c'\n", post[i]);
                    DisposeNum(s);
                    return 0;
            }


            PushNum(result, s);
        }
    }
        // Ensure there is at least one value in the stack before getting the final result
        if (IsEmptyNum(s)) {
           printf("invalid postfix expression\n");
           DisposeNum(s);
           return 0;
        }

       int Res=TopNum(s);
       PopNum(s);
      // Ensure no extra elements remain in the stack
      if(!IsEmptyNum(s)) {
        printf("Invalid postfix expression\n");
        DisposeNum(s);
        return 0;
     }

    DisposeNum(s);
    return Res;

}

// Build the expression tree from postfix string
TreeNode BuildTree(char post[]) {

    StackTree S=CreateStackTree();
    char temp[MAX_LEN];
    int k;

    for (int i=0; post[i]!='\0'; i++) {

        if (post[i]==' '|| post[i]=='\t' )
            continue;
       // if post[i] number,should make a node and push it to stack
       if (isdigit(post[i]) || (post[i] == '-' && isdigit(post[i+1]))) {
           k = 0;
         if (post[i] == '-') {
            temp[k++] = post[i++];
          }
         while (isdigit(post[i])) {
            temp[k++] = post[i++];
       }
            temp[k] = '\0';
             i--;

        TreeNode newNode=CreateTreeNode(temp);
        PushTree(newNode, S);
  }
        // if operator, pop two trees and make them children of the operator node
        else if (IsOp(post[i])) {

                if (IsEmptyTree(S)) {
                    printf("Invalid postfix expression\n");
                    return NULL;
                }
                 TreeNode right = TopTree(S);
                 PopTree(S);

               if (IsEmptyTree(S)) {
                   printf("Invalid postfix expression\n");
                   return NULL;
               }
              TreeNode left=TopTree(S);
              PopTree(S);

              char op[2];
              op[0] = post[i];
              op[1] = '\0';

              TreeNode opNode = CreateTreeNode(op);
              opNode->left = left;
              opNode->right = right;
              PushTree(opNode, S);

        }
    }

     if (IsEmptyTree(S)) {
    printf("Invalid Postfix exprssion!\n");
    free(S);
    return NULL;
}

   TreeNode root=TopTree(S);
   PopTree(S);

   if (!IsEmptyTree(S)) {
    printf("Invalid postfix expression!\n");
    free(S);
    return NULL;
}

   free(S);
   return root;
}

// inorder travrsal to print infix format & brackets
void inorder(TreeNode root) {

    if (root == NULL)
        return;

    int isOp=(root->left != NULL && root->right != NULL);

    if (isOp)
        printf("(");

    inorder(root->left);
    printf("%s ", root->data);
    inorder(root->right);

     if (isOp)
        printf(")");
}
// preorder traversal to print prefix format
void preorder(TreeNode root) {

    if (root == NULL)
        return;

    printf("%s ", root->data);
    preorder(root->left);
    preorder(root->right);
}

// postorder traversal to print Postfix format
void postorder(TreeNode root) {

    if (root == NULL)
        return;

    postorder(root->left);
    postorder(root->right);
    printf("%s ", root->data);
}

// write results to output.txt following the project requirements
void wOutputFile(List L) {

    FILE *file=fopen("output.txt", "w");

    if (file == NULL) {
        printf("Error: Could not open output.txt\n");
        return;
    }

    char pfix[MAX_LEN];
    char errorMsg[200];
    char eqClean[MAX_LEN];

    Position p=L->next;
    int i=1;
    // loop through all equations and write details to file
    while (p != NULL) {
         if (IsValid(p->equation, errorMsg)) {

            removeSpace(p->equation, eqClean);

            infixToPost(eqClean, pfix);
            int res = evaluatePostfix(pfix);
            fprintf(file,"Equation No. %d --> valid\nPostfix: %s\nResult = %d\n\n",i, pfix, res);

        } else {

            fprintf(file,
                "Equation No. %d --> invalid: %s\n\n",
                i, errorMsg);
        }
        p = p->next;
        i++;
    }

     fclose(file);
     printf("Results successfully saved to output.txt\n");

}

