#include "m_pd.h"
#include <string.h>
#include <dsp.h>

static t_class *rp_delay_tilde_class;

typedef struct _rp_delay_tilde
{
    t_object x_obj;

    t_inlet* x_in;
    t_outlet* x_out;
    Delay* delay;
} t_rp_delay_tilde;

t_int* rp_delay_tilde_perform(t_int *w)
{
    t_rp_delay_tilde* x = (t_rp_delay_tilde *)(w[1]);
    t_sample* in = (t_sample*)(w[2]);
    t_sample* out = (t_sample*)(w[3]);
    uintptr_t size = (uintptr_t)(w[4]);

    memcpy(out, in, sizeof(float) * size);
    delay_process(x->delay, out, size);

    return (w+5);
}

void rp_delay_tilde_dsp(t_rp_delay_tilde* x, t_signal **sp)
{
    dsp_add(rp_delay_tilde_perform, 4,
            x,
            sp[0]->s_vec,
            sp[1]->s_vec,
            sp[0]->s_n);
}

void *rp_delay_tilde_new(t_float delay_in_samples)
{
    t_rp_delay_tilde *x = (t_rp_delay_tilde *)pd_new(rp_delay_tilde_class);
    x->x_in = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
    x->x_out = outlet_new(&x->x_obj, &s_signal);
    x->delay = delay_create((uintptr_t) delay_in_samples);
    return (void *)x;
}

void rp_delay_tilde_free(t_rp_delay_tilde *x)
{
    inlet_free(x->x_in);
    outlet_free(x->x_out);
    delay_destroy(x->delay);
}

void rp_delay_tilde_setup(void) {
    rp_delay_tilde_class = class_new(gensym("rp_delay~"),
                                   (t_newmethod)rp_delay_tilde_new,
                                   (t_method)rp_delay_tilde_free,
                                   sizeof(t_rp_delay_tilde), CLASS_NOINLET, A_FLOAT, 0);

    class_addmethod(rp_delay_tilde_class, (t_method)rp_delay_tilde_dsp, gensym("dsp"), A_CANT, 0);
}