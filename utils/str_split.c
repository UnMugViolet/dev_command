#include <stdlib.h>
#include <ctype.h>
#include "utils.h"

int is_charset(char c, char *charset)
{
  int i;

  i = 0;
  while (charset[i])
  {
    if (c == charset[i])
      return (1);
    i++;
  }
  return (0);
}

int count_words(char *str, char *charset)
{
  int i = 0;
  int count = 0;

  while (str[i])
  {
    // Skip leading spaces and delimiters
    while (str[i] && (isspace(str[i]) || is_charset(str[i], charset)))
      i++;

    if (str[i] && !is_charset(str[i], charset) && !isspace(str[i]))
    {
      count++;
      while (str[i] && !is_charset(str[i], charset) && !isspace(str[i]))
        i++;
    }
  }
  return count;
}

int ft_word_len(char *str, char *charset)
{
  int i = 0;
  int counter = 0;

  while (str[i] && (isspace(str[i]) || is_charset(str[i], charset)))
    i++;

  while (str[i] && !is_charset(str[i], charset) && !isspace(str[i]))
  {
    counter++;
    i++;
  }

  return counter;
}

void ft_strcpy(char *str, char *charset, char *array)
{
  int i;
  int j;

  i = 0;
  j = 0;
  while (str[i] && (isspace(str[i]) || is_charset(str[i], charset)))
    i++;

  while (str[i] && !is_charset(str[i], charset) && !isspace(str[i]))
  {
    array[j] = str[i];
    i++;
    j++;
  }
  array[j] = 0;
}

char **str_split(char *str, char *charset)
{
  char **array;
  int next_word = 0;
  int i = 0;
  int word_count = count_words(str, charset);

  array = (char **)malloc(sizeof(char *) * (word_count + 1));
  if (!array)
    return NULL;

  while (i < word_count)
  {
    int word_len = ft_word_len(str + next_word, charset);
    array[i] = (char *)malloc(sizeof(char) * (word_len + 1));
    if (!array[i])
      return NULL;
    ft_strcpy(str + next_word, charset, array[i]);
    next_word += word_len;

    // Skip trailing spaces and delimiters
    while (str[next_word] && (isspace(str[next_word]) || is_charset(str[next_word], charset)))
      next_word++;
    i++;
  }
  array[i] = NULL;
  return array;
}

// #include <stdio.h>

// int main(int ac, char **av)
// {
//   (void)ac;
//   unsigned long int count = count_words(av[1], av[2]);
//   char **result;

//   result = str_split(av[1], av[2]);

//   printf("Output :\n");
//   for (unsigned long int i = 0; i < count; i++)
//   {
//     printf("%s\n", result[i]);
//     free(result[i]);
//   }
//   free(result);
// }