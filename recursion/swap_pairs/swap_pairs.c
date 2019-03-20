#include <stdio.h>
#include <stdlib.h>

struct ListNode {
        int val;
        struct ListNode *next;
};

struct ListNode* push(struct ListNode* node, int val)
{
        struct ListNode *new_node = (struct ListNode*)malloc(sizeof(struct ListNode));
        new_node->val = val;
        new_node->next = node;
        return new_node;
}

void print_list(struct ListNode* head)
{
        while (head != NULL) {
                printf("%d->", head->val);
                head = head->next;
        }
        printf("NULL\n");
}

struct ListNode* swap_pairs(struct ListNode* head)
{
        if (head == NULL || head->next == NULL)
                return head;
        struct ListNode *temp = head->next->next;
        struct ListNode *res = head->next;
        head->next->next = head;
        head->next = swap_pairs(temp);
        return res;
}


int main(int argc, char *argv[])
{
        struct ListNode *head = NULL;
        for (int i = 4; i >= 1; i--) {
                head = push(head, i);
        }
        printf("Original list: ");
        print_list(head);
        head = swap_pairs(head);
        printf("After swap: ");
        print_list(head);
}
