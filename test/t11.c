int main () {
  int i,j;
  i = 0;
  j = 0;
  while (i < 5) {
    j = 0;
    while (j < 4) {
      j = j + 1;
      if (j == 2) i = 1;
      else i = 2;
    }
    i = i + 1;
  }
  return i;
}
