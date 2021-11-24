#include "m_pd.h"
#include <string.h>
#include <dsp.h>

static t_class *rp_additive_tilde_class;

typedef struct _rp_additive_tilde
{
    t_object x_obj;

    t_outlet* x_out;
    Additive* additive;

    t_float f;
} t_rp_additive_tilde;

t_int* rp_additive_tilde_perform(t_int *w)
{
    t_rp_additive_tilde* x = (t_rp_additive_tilde *)(w[1]);
    t_sample* in = (t_sample*)(w[2]);
    t_sample* out = (t_sample*)(w[3]);
    uintptr_t size = (uintptr_t)(w[4]);

    memcpy(out, in, sizeof(float) * size);
    additive_process(x->additive, out, size);

    return (w+5);
}

void rp_additive_tilde_dsp(t_rp_additive_tilde* x, t_signal **sp)
{
    dsp_add(rp_additive_tilde_perform, 4,
            x,
            sp[0]->s_vec,
            sp[1]->s_vec,
            sp[0]->s_n);
}

void *rp_additive_tilde_new()
{
    t_rp_additive_tilde *x = (t_rp_additive_tilde *)pd_new(rp_additive_tilde_class);
    x->x_out = outlet_new(&x->x_obj, &s_signal);
    x->additive = additive_create();
    return (void *)x;
}

void rp_additive_tilde_free(t_rp_additive_tilde *x)
{
    outlet_free(x->x_out);
    additive_destroy(x->additive);
}

void rp_additive_tilde_harm(t_rp_additive_tilde *x, t_float index, t_float amp)
{
    additive_harm(x->additive, (uintptr_t)index, amp);
}

void rp_additive_tilde_setup(void) {
    rp_additive_tilde_class = class_new(gensym("rp_additive~"),
                                     (t_newmethod)rp_additive_tilde_new,
                                     (t_method)rp_additive_tilde_free,
                                     sizeof(t_rp_additive_tilde), CLASS_DEFAULT, 0);

    class_addmethod(rp_additive_tilde_class, (t_method)rp_additive_tilde_dsp, gensym("dsp"), A_CANT, 0);

    class_addmethod(rp_additive_tilde_class, (t_method)rp_additive_tilde_harm, gensym("harm"), A_FLOAT, A_FLOAT, 0);

    CLASS_MAINSIGNALIN(rp_additive_tilde_class,t_rp_additive_tilde, f);
}