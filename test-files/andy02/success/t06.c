int main () {
  int global[10];
  global[0] = 0;
  int i;
  i = 0;
  while (i < 10) {
    global[i] = i;
    i = i + 1;
  }
  return i + 12;
}
