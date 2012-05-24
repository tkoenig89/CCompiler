int main () {
  int i;
  i = 0;
  do {
    i = i << 2;
  } while (i < 1024 && i > 512);
  return i;
}
