#ifndef SIM_MATRIX_H_
#define SIM_MATRIX_H_
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "platform.h"
#include "simd/matrix4xm128f.h"
#include "vectorf.h"

typedef __m128 mat4xm128[4];

typedef union mat4x4
{
    SIM_ALIGN_16 float values[4][4];
    __m128 sse_registers[4];
}mat4_t;


void mat4_rotate(mat4_t *out, mat4_t const *input, fvec3 const* axis, float angle_rad)
{
    SIM_ASSERT(is_vec3(axis) && input != NULL && out != NULL);
    mat4xm128_rotate(input->sse_registers, axis->data.sse_register, angle_rad, out->sse_registers);
}

void mat4_translate(mat4_t *output, mat4_t const *input, fvec3 const *vec)
{
    SIM_ASSERT(is_vec3(vec) && input != NULL && output != NULL);
    mat4xm128_translate(input->sse_registers, vec->data.sse_register, output->sse_registers);
}

void vec4_outer_product(mat4_t *output, fvec4 const *input0,  fvec4 const *input1)
{
    SIM_ASSERT(output != NULL);
    SIM_ASSERT(is_vec4(input0) && is_vec4(input1));

    mat4xm128_outer_product(input0->data.sse_register, input1->data.sse_register,
                            output->sse_registers);
}

void mat4_vec_product(fvec4 *output, mat4_t const *mat, fvec4 const *vec)
{
    SIM_ASSERT(vec->size == 4);
    output->size = vec->size;
    output->data.sse_register = mat4xm128_vec4_product(mat->sse_registers, vec->data.sse_register);
}

void mat4_init_diagonal(mat4_t *output, const float translator)
{
    SIM_ASSERT(output != NULL);

    output->sse_registers[0] = _mm_set_ps(0.0f, 0.0f, 0.0f, translator);
    output->sse_registers[1] = _mm_set_ps(0.0f, 0.0f, translator, 0.0f);
    output->sse_registers[2] = _mm_set_ps(0.0f, translator, 0.0f, 0.0f);
    output->sse_registers[3] = _mm_set_ps(translator, 0.0f, 0.0f, 0.0f);
}

void mat4_init_translation(mat4_t *output, float x, float y, float z)
{
    SIM_ASSERT(output != NULL);

    output->sse_registers[0] = _mm_set_ps(x, 0.0f, 0.0f, 1.0f);
    output->sse_registers[1] = _mm_set_ps(y, 0.0f, 1.0f, 0.0f);
    output->sse_registers[2] = _mm_set_ps(z, 1.0f, 0.0f, 0.0f);
    output->sse_registers[3] = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
}

void mat4_print(const mat4_t *mat)
{
    SIM_ASSERT(mat != NULL);

    for(uint32_t i = 0; i < 4; i++)
    {
        printf("\n");
        for(uint32_t j = 0; j < 4; j++){
            if(i == 0 && j == 0) printf("[");
            printf("%f ", mat->values[i][j]);
        }
    }
    printf("] \n");
}

/* Primitve function-like-macros that aren't worth any overhead: */
void mat4_mul(mat4_t out, mat4_t const in0, mat4_t const in1);
void mat4_add(mat4_t out, mat4_t const in0, mat4_t const in1);
void mat4_sub(mat4_t out, mat4_t const in0, mat4_t const in1);
void mat4_transpose(mat4_t out, mat4_t const in);

#define mat4_mul(out, in0, in1)                 \
    mat4xm128_mul((in0).sse_registers, (in1).sse_registers, (out).sse_registers) \

#define mat4_add(out, in0, in1)                 \
    mat4xm128_add((in0).sse_registers, (in1).sse_registers, (out).sse_registers) \

#define mat4_sub(out, in0, in1)                 \
    mat4xm128_sub((in0).sse_registers, (in1).sse_registers, (out).sse_registers) \

#define mat4_transpose(out, in0)                \
    mat4xm128_transpose((in0).sse_registers, (out).sse_registers) \

#endif // SMATH_MATRICES_H_
