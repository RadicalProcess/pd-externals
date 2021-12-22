#include "m_pd.h"
#include <string.h>
#include <dsp.h>

static t_class *rp_xfader_tilde_class;

typedef struct _rp_xfader_tilde
{
    t_object x_obj;
    t_inlet* x_in2;
    t_outlet* x_out;
    XFader* xFader;

    t_float f;
} t_rp_xfader_tilde;

t_int* rp_xfader_tilde_perform(t_int *w)
{
    t_rp_xfader_tilde* x = (t_rp_xfader_tilde *)(w[1]);
    t_sample* in1 = (t_sample*)(w[2]);
    t_sample* in2 = (t_sample*)(w[3]);
    t_sample* out = (t_sample*)(w[4]);
    uintptr_t size = (uintptr_t)(w[5]);

    memcpy(out, in2, sizeof(float) * size);
    xfader_process(x->xFader, out, in1, size);

    return (w+6);
}

void rp_xfader_tilde_dsp(t_rp_xfader_tilde* x, t_signal **sp)
{
    dsp_add(rp_xfader_tilde_perform, 5,
            x,
            sp[0]->s_vec,
            sp[1]->s_vec,
            sp[2]->s_vec,
            sp[0]->s_n);
}

void *rp_xfader_tilde_new(void)
{
    t_rp_xfader_tilde *x = (t_rp_xfader_tilde *)pd_new(rp_xfader_tilde_class);

    x->x_in2 = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);

    x->x_out = outlet_new(&x->x_obj, &s_signal);
    x->xFader = xfader_create(0.5f);

    return (void *)x;
}

void rp_xfader_tilde_free(t_rp_xfader_tilde *x)
{
    inlet_free(x->x_in2);
    outlet_free(x->x_out);

    xfader_destroy(x->xFader);
}

void rp_xfader_tilde_mix(t_rp_xfader_tilde *x, t_float mix){
    xfader_set_dry_wet(x->xFader, mix);
}


void rp_xfader_tilde_setup(void) {
    rp_xfader_tilde_class = class_new(gensym("rp_xfader~"),
                                     (t_newmethod)rp_xfader_tilde_new,
                                     (t_method)rp_xfader_tilde_free,
                                     sizeof(t_rp_xfader_tilde), CLASS_DEFAULT, 0);


    class_addmethod(rp_xfader_tilde_class, (t_method)rp_xfader_tilde_dsp, gensym("dsp"), A_CANT, 0);
    class_addmethod(rp_xfader_tilde_class, (t_method)rp_xfader_tilde_mix, gensym("mix"), A_FLOAT, 0);

    CLASS_MAINSIGNALIN(rp_xfader_tilde_class, t_rp_xfader_tilde, f);
}
