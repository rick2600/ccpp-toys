#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct _str {
  char *s;
  struct _str *next;
} str_t;


typedef struct _hashtbl {
  str_t **slots;
  unsigned int n_slots;
} hashtbl_t;


// Ref: http://www.cse.yorku.ca/~oz/hash.html
unsigned long djb2(unsigned char *str)
{
  unsigned long hash = 5381;
  int c;

  while (c = *str++)
    hash = ((hash << 5) + hash) + c;

  return hash;
}


str_t *create_entry(char *str)
{
  str_t *temp = malloc(sizeof(str_t));
  if (!temp)
  {
    perror("");
    exit(EXIT_FAILURE);
  }
  temp->s = strdup(str);
  temp->next = NULL;
  return temp;
}


void insert_str(hashtbl_t *hash_table, char *str)
{
  str_t *entry;
  unsigned long pos = (djb2(str) % hash_table->n_slots);

  if (!str_is_present(hash_table, str))
  {
    entry = create_entry(str);
    if (hash_table->slots[pos])
      entry->next = hash_table->slots[pos];
    
    hash_table->slots[pos] = entry;
  }
  else
    printf("duplicated entry is not allowed!\n");
}


int str_is_present(hashtbl_t *hash_table, char *str)
{
  unsigned long pos = (djb2(str) % hash_table->n_slots);
  int found = 0;
  str_t *aux = hash_table->slots[pos];

  while (aux) 
  {
    if (!strcmp(aux->s, str))
    {
      found = 1;
      break;
    }
    aux = aux->next;
  }
  return found;
}


hashtbl_t *create_hash_table(unsigned int n)
{
  hashtbl_t *hash_table = malloc(sizeof(hashtbl_t));
  if (!hash_table)
  {
    perror("");
    exit(EXIT_FAILURE);
  }

  hash_table->slots = malloc(sizeof(str_t *) * n);
  if (!(hash_table->slots))
  {
    perror("");
    exit(EXIT_FAILURE);
  }
  hash_table->n_slots = n;

  memset(hash_table->slots, 0, (sizeof(str_t *) * n));
  return hash_table;
}

void destroy_hash_table(hashtbl_t *hash_table)
{
  str_t *cur, *aux;
  unsigned int i;

  for (i = 0; i < hash_table->n_slots; i++)
  {
    if (hash_table->slots[i])
    {
      cur = hash_table->slots[i];
      while (cur)
      {
        aux = cur;
        cur = cur->next;
        
        free(aux->s);
        aux->s = NULL;
        aux->next = NULL;
        free(aux);        
      }
    }
  }
  memset(hash_table->slots, 0, (sizeof(str_t *) * hash_table->n_slots));
  free(hash_table->slots);
  free(hash_table);
}

int main(int argc, char **argv)
{
  hashtbl_t *hash_table = create_hash_table(5000);

  insert_str(hash_table, "abacaxi");

  if (str_is_present(hash_table, "abacaxi"))
    printf("found!\n");
  else
    printf("not found\n");

  destroy_hash_table(hash_table);  

  exit(EXIT_SUCCESS);
}
