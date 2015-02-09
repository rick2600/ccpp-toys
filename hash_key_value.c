#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct _key_value {
  char *key;
  char *value;
  struct _key_value *next;
} key_value_t;


typedef struct _hashtbl {
  key_value_t **slots;
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


key_value_t *create_entry(char *key, char *value)
{
  key_value_t *temp = malloc(sizeof(key_value_t));
  if (!temp)
  {
    perror("");
    exit(EXIT_FAILURE);
  }
  temp->key = strdup(key);
  temp->value = strdup(value);
  temp->next = NULL;
  return temp;
}


char *get_value(hashtbl_t *hash_table, char *key)
{
  unsigned long pos = (djb2(key) % hash_table->n_slots);
  int found = 0;
  key_value_t *aux = hash_table->slots[pos];

  while (aux) 
  {
    if (!strcmp(aux->key, key))
      return aux->value;

    aux = aux->next;
  }
  return NULL;
}

key_value_t *entry_exist(hashtbl_t *hash_table, char *key)
{
  unsigned long pos = (djb2(key) % hash_table->n_slots);
  int found = 0;
  key_value_t *aux = hash_table->slots[pos];

  while (aux) 
  {
    if (!strcmp(aux->key, key))
      return aux;

    aux = aux->next;
  }
  return NULL;
}

void insert_key_value(hashtbl_t *hash_table, char *key, char *value)
{
  key_value_t *entry;
  unsigned long pos = (djb2(key) % hash_table->n_slots);

  entry = entry_exist(hash_table, key);
  if (!entry)
  {
    entry = create_entry(key, value);

    if (hash_table->slots[pos])
      entry->next = hash_table->slots[pos];
    
    hash_table->slots[pos] = entry;
  }
  else
  {
    free(entry->value);
    entry->value = strdup(value);
  }
}


hashtbl_t *create_hash_table(unsigned int n)
{
  hashtbl_t *hash_table = malloc(sizeof(hashtbl_t));
  if (!hash_table)
  {
    perror("");
    exit(EXIT_FAILURE);
  }

  hash_table->slots = malloc(sizeof(key_value_t *) * n);
  if (!(hash_table->slots))
  {
    perror("");
    exit(EXIT_FAILURE);
  }
  hash_table->n_slots = n;

  memset(hash_table->slots, 0, (sizeof(key_value_t *) * n));
  return hash_table;
}

void destroy_hash_table(hashtbl_t *hash_table)
{
  key_value_t *cur, *aux;
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
        
        free(aux->key);
        free(aux->value);
        aux->key = NULL;
        aux->value = NULL;
        aux->next = NULL;
        free(aux);        
      }
    }
  }
  memset(hash_table->slots, 0, (sizeof(key_value_t *) * hash_table->n_slots));
  free(hash_table->slots);
  free(hash_table);
}

int main(int argc, char **argv)
{
  hashtbl_t *hash_table = create_hash_table(5000);
  char *value;

  insert_key_value(hash_table, "key", "_V@lu3_");

  value = get_value(hash_table, "key");
  if (value)
    printf("%s\n", value);

  insert_key_value(hash_table, "key", "_V@lu3_updated_");

  value = get_value(hash_table, "key");
  if (value)
    printf("%s\n", value);

  value = get_value(hash_table, "not_exist");
  if (value)
    printf("%s\n", value);


  destroy_hash_table(hash_table);  

  exit(EXIT_SUCCESS);
}
