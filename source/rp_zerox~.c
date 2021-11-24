#include "m_pd.h"
#include <dsp.h>
#include <string.h>

static t_class *rp_zerox_tilde_class;

typedef struct _rp_zerox_tilde
{
    t_object x_obj;
    t_outlet* x_out;
    ZeroX * zeroX;

    t_float f;
} t_rp_zerox_tilde;

t_int* rp_zerox_tilde_perform(t_int *w)
{
    t_rp_zerox_tilde* x = (t_rp_zerox_tilde *)(w[1]);
    t_sample* in = (t_sample*)(w[2]);
    t_sample* out = (t_sample*)(w[3]);
    uintptr_t size = (uintptr_t)(w[4]);
    memcpy(out, in, sizeof(float) * size);
    zerox_process(x->zeroX, out, size);
    return (w+5);
}

void rp_zerox_tilde_dsp(t_rp_zerox_tilde* x, t_signal **sp)
{
    dsp_add(rp_zerox_tilde_perform, 4,
            x,
            sp[0]->s_vec,
            sp[1]->s_vec,
            sp[0]->s_n);
}

void *rp_zerox_tilde_new(t_float timeout)
{
    t_rp_zerox_tilde *x = (t_rp_zerox_tilde *)pd_new(rp_zerox_tilde_class);
    x->x_out = outlet_new(&x->x_obj, &s_signal);
    x->zeroX = zerox_create((uintptr_t) timeout);
    return (void *)x;
}

void rp_zerox_tilde_free(t_rp_zerox_tilde *x)
{
    outlet_free(x->x_out);
    zerox_destroy(x->zeroX);
}

void rp_zerox_skip(t_rp_zerox_tilde *x, t_float f)
{
    zerox_set_skip(x->zeroX, (uintptr_t) f);
}

void rp_zerox_min_length(t_rp_zerox_tilde *x, t_float f)
{
    zerox_set_min_length(x->zeroX, (uintptr_t) f);
}

void rp_zerox_tilde_setup(void) {
    rp_zerox_tilde_class = class_new(gensym("rp_zerox~"),
                                     (t_newmethod)rp_zerox_tilde_new,
                                     (t_method)rp_zerox_tilde_free,
                                     sizeof(t_rp_zerox_tilde), CLASS_DEFAULT, A_FLOAT, 0);

    class_addmethod(rp_zerox_tilde_class, (t_method)rp_zerox_tilde_dsp, gensym("dsp"), A_CANT, 0);

    class_addmethod(rp_zerox_tilde_class, (t_method)rp_zerox_skip, gensym("skip"), A_FLOAT, 0);
    class_addmethod(rp_zerox_tilde_class, (t_method)rp_zerox_min_length, gensym("min_length"), A_FLOAT, 0);

    CLASS_MAINSIGNALIN(rp_zerox_tilde_class, t_rp_zerox_tilde, f);
}
