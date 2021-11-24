#include "m_pd.h"
#include <stdlib.h>

static t_class *rp_test_tilde_class;

typedef struct _rp_test_tilde
{
    t_object x_obj;
    t_outlet* x_out;
} t_rp_test_tilde;

t_int* rp_test_tilde_perform(t_int *w)
{
    t_rp_test_tilde* x = (t_rp_test_tilde *)(w[1]);
    t_sample* data = (t_sample*)(w[2]);
    int size = (int)(w[3]);

    for(int i = 0; i < size; ++i)
        data[i] = (float)rand()/(float)(RAND_MAX) - 0.5f;

    return (w+4);
}

void rp_test_tilde_dsp(t_rp_test_tilde* x, t_signal **sp)
{
    dsp_add(rp_test_tilde_perform, 3,
            x,
            sp[0]->s_vec, sp[0]->s_n);
}

void *rp_test_tilde_new(void)
{
    t_rp_test_tilde *x = (t_rp_test_tilde *)pd_new(rp_test_tilde_class);
    x->x_out = outlet_new(&x->x_obj, &s_signal);
    return (void *)x;
}

void rp_test_tilde_free(t_rp_test_tilde *x)
{
    outlet_free(x->x_out);
}

void rp_test_tilde_setup(void) {
    rp_test_tilde_class = class_new(gensym("rp_test~"),
                                       (t_newmethod)rp_test_tilde_new,
                                       (t_method)rp_test_tilde_free,
                                       sizeof(t_rp_test_tilde), CLASS_NOINLET, 0);

    class_addmethod(rp_test_tilde_class,
                    (t_method)rp_test_tilde_dsp, gensym("dsp"), A_CANT, 0);

}