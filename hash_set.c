// HASH SET //
typedef struct HashSetNode {
  void *data;
  struct HashSetNode *next;
} HashSetNode;

typedef struct HashSet {
  HashSetNode **table;
  size_t size;
  size_t capacity;
  size_t (*hash)(void *);
  int (*equals)(void *, void *);
} HashSet;

size_t default_hash(void *key) {
  return (size_t)key % 256;
}

int default_equals(void *a, void *b) {
  return a == b;
}

HashSet *hashset_create(size_t capacity, size_t (*hash)(void *), int (*equals)(void *, void *))
{
  HashSet *set = malloc(sizeof(HashSet));
  set->capacity = capacity;
  set->size = 0;
  set->hash = hash ? hash : default_hash;
  set->equals = equals ? equals : default_equals;
  set->table = calloc(capacity, sizeof(HashSetNode *));
  return set;
}

void hashset_free_nodes(HashSetNode *node)
{
  while (node) {
    HashSetNode *temp = node;
    node = node->next;
    free(temp);
  }
}

void hashset_free(HashSet *set)
{
  for (size_t i = 0; i < set->capacity; i++) {
    hashset_free_nodes(set->table[i]);
  }
  free(set->table);
  free(set);
}

void hashset_resize(HashSet *set)
{
  size_t new_capacity = set->capacity * 2;
  HashSetNode **new_table = calloc(new_capacity, sizeof(HashSetNode *));
  for (size_t i = 0; i < set->capacity; i++) {
    HashSetNode *node = set->table[i];
    while (node) {
      HashSetNode *next = node->next;
      size_t index = set->hash(node->data) % new_capacity;
      node->next = new_table[index];
      new_table[index] = node;
      node = next;
    }
  }
  free(set->table);
  set->table = new_table;
  set->capacity = new_capacity;
}

void hashset_insert(HashSet *set, void *data)
{
  if (set->size >= set->capacity * 0.75) {
    hashset_resize(set);
  }
  size_t index = set->hash(data) % set->capacity;
  HashSetNode *node = set->table[index];
  while (node) {
    if (set->equals(node->data, data)) {
      return;
    }
    node = node->next;
  }
  HashSetNode *new_node = malloc(sizeof(HashSetNode));
  new_node->data = data;
  new_node->next = set->table[index];
  set->table[index] = new_node;
  set->size++;
}

void hashset_remove(HashSet *set, void *data)
{
  size_t index = set->hash(data) % set->capacity;
  HashSetNode *node = set->table[index];
  HashSetNode *prev = NULL;

  while (node) {
    if (set->equals(node->data, data)) {
      if (prev) {
        prev->next = node->next;
      } else {
        set->table[index] = node->next;
      }
      free(node);
      set->size--;
      return;
    }
    prev = node;
    node = node->next;
  }
}

int hashset_contains(HashSet *set, void *data)
{
  size_t index = set->hash(data) % set->capacity;
  HashSetNode *node = set->table[index];
  while (node) {
    if (set->equals(node->data, data)) {
      return 1;
    }
    node = node->next;
  }
  return 0;
}
