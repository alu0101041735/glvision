
typedef struct Rgba
{
    int red;
    int green;
    int blue;
    int a;
}Rgba;


__kernel void kernel grayscale(__global Rgba *A, unsigned long int m_size, __global Rgba *C){
    if (get_global_id(0) < m_size) {

        float aux = (float) A[get_global_id(0)].red;
        C[get_global_id(0)].red = (int) (aux * 0.299);

        aux = (float) A[get_global_id(0)].green;
        C[get_global_id(0)].green = (int) (aux * 0.587);

        aux = (float) A[get_global_id(0)].blue;
        C[get_global_id(0)].blue = (int) (aux * 0.114);

        C[get_global_id(0)].a = 255;
    }

    if (get_global_id(0) == 1000000) {
        printf("Original color: %d,%d,%d\nGray color: %d,%d,%d\n",
               A[1000000].red, A[1000000].green, A[1000000].blue,
                C[1000000].red, C[1000000].green, A[1000000].blue);
    }
}
