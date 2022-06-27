#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include<time.h>

#define ITEMS_COUNT 20

typedef struct item{
    uint16_t number;
    char description[251];
    double weight;
    double price;
}item;

typedef struct Node{
    item data;
    struct Node* next;
}node_t;

double randReal(double,double);
void createNode(node_t **);
void printNode(node_t *); //head няма да го променяме, затова изпозлваме само една *
void push_front(node_t **, item);
item pop_front(node_t **);
void clearList(node_t **);
double totalItemsWeight(node_t *);
node_t *minPriceItem(node_t *);

int main(){
    srand(time(NULL));
    node_t *head = NULL;
    createNode(&head);
    printNode(head);
    printf("\n\nTotal weight is: %.2lf\n\n", totalItemsWeight(head));
    node_t *cheapest = minPriceItem(head);
    printf("\n\nCheapest item is: (%u, %s, %.2lf, %.2lf)\n\n\n",
           cheapest->data.number, cheapest->data.description, cheapest->data.weight, cheapest->data.price);
    clearList(&head);

    return 0;
}

void createNode(node_t **head){
    int unique[ITEMS_COUNT];
    for (int i = 0; i < ITEMS_COUNT; i++)
    {
        item push_item;
        int unique_num = rand();
        for(int j = 0 ; j < i; j++)
        {
            if(unique_num == unique[j])
            {
                int unique_num = rand();
                j = 0;
            }
        }
        push_item.number = unique_num;

        int desc_words = 5 + rand() % 6;
        push_item.description[0] = 'A' + rand() % ('Z' - 'A' + 1);
        int word_len = 1 + rand() % 10;

        for (int k = 1; k < word_len; k++)
        {
            push_item.description[k] = 'a' + rand() % ('z' - 'a' + 1);
        }
        push_item.description[word_len] = ' ';

        int index = word_len + 1;
        for (int j = 1; j < desc_words; j++)
        {
            word_len = 1 + rand() % 10;
            for (int k = 0; k < word_len; k++)
            {
                push_item.description[index++] = 'a' + rand() % ('z' - 'a' + 1);
            }
            push_item.description[index++] = ' ';
        }
        push_item.description[index++] = '\0';

        push_item.weight = randReal(0.100, 100.00);

        push_item.price = randReal(1.00, 1500.00);

        push_front(head, push_item);
    }

}

double randReal(double min, double max){
    return min + ((double)rand() / RAND_MAX) * (max - min);
}

void printNode(node_t *head){
    node_t* temporary = head;
    while(temporary != NULL){
        printf("Number: %5u ", temporary->data.number);
        printf("Description: %25s ", temporary->data.description);
        printf("Weight: %4.2lf ", temporary->data.weight);
        printf("Price: %4.2lf\n", temporary->data.price);

        temporary = temporary->next;
    }
    printf("\n");
}

double totalItemsWeight(node_t *head){
    int total = 0;
    if (!head){
        return -1;
    }
    while(head){
        total += head->data.weight;
        head = head->next;
    }
    return total;
}

node_t *minPriceItem(node_t *head){
    if (NULL == head)
    {
        return NULL;
    }
    node_t *min_node = head;
    double min_price = head->data.price;

    head = head->next;
    while (head != NULL)
    {
        if (head->data.price < min_price)
        {   
            min_price = head->data.price;
            min_node = head;
        }
        head = head->next;
    }

    return min_node;

}

void push_front(node_t **head, item value)
{
    node_t *newnode = malloc(sizeof(node_t));

    newnode->data = value;
    newnode->next = *head;

    *head = newnode;
}

item pop_front(node_t **head)
{
    if (!(*head))
        exit(-1);

    node_t *temp = *head;
    item result = temp->data;
    *head = (*head)->next;

    free(temp);
    return result;
}

void clearList(node_t **head)
{
    while (*head)
    {
        pop_front(head);
    }
}


