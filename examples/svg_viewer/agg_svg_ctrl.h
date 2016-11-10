#ifndef SVG_CTRL_
#define SVG_CTRL_

#include "agg_path_storage.h"
#include "agg_conv_transform.h"
#include "agg_conv_stroke.h"
#include "agg_conv_contour.h"
#include "agg_conv_curve.h"
#include "agg_color_rgba.h"
#include "agg_renderer_scanline.h"
#include "agg_bounding_rect.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_svg_path_tokenizer.h"

#include "agg_svg_path_renderer.h"
#include "agg_svg_parser.h"


class SvgCtrl
{
public:
    SvgCtrl(const char* fname)
    {
        agg::svg::parser p(m_path);
        p.parse(fname);
        m_path.arrange_orientations();
        m_path.arrange_orientations();
        m_path.bounding_rect(&m_min_x, &m_min_y, &m_max_x, &m_max_y);
    }
    template<class Rasterizer, class Scanline, class Renderer> 
    void render(Rasterizer& ras, 
        Scanline& sl,
        Renderer& ren, 
        const agg::trans_affine& mtx, 
        const agg::rect_i& cb,
        double opacity=1.0)
    {
        m_path.render(ras, sl, ren, mtx, cb, opacity);
    }
protected:
    double m_min_x;
    double m_min_y;
    double m_max_x;
    double m_max_y;

    agg::svg::path_renderer m_path;
};

class CtrlContainer : public SvgCtrl
{
public:
    CtrlContainer(const char* path):SvgCtrl(path)
    {
        pod_ctrl.capacity(10);
    }
    void AddCtrl(SvgCtrl* c)
    {
        pod_ctrl.push_back(c);
    }
protected:
    agg::pod_vector<SvgCtrl*> pod_ctrl;
};


class HorizontalLayout : public CtrlContainer
{
public:
    HorizontalLayout(const char* path):CtrlContainer(path){gap = 3;}

    template<class Rasterizer, class Scanline, class Renderer> 
    void render(Rasterizer& ras, 
        Scanline& sl,
        Renderer& ren, 
        const agg::trans_affine& mtx, 
        const agg::rect_i& cb,
        double opacity=1.0)
    {
        int EachCtrlSize = ((cb.x2 - cb.x1) - pod_ctrl.size()*gap)/pod_ctrl.size();
        for (int i = 0; i < pod_ctrl.size(); i++)
        {
            agg::rect_i rc;
            rc.x1 = EachCtrlSize*i;
            rc.x2 = rc.x1 + EachCtrlSize;
            rc.y1 = cb.y1;
            rc.y2 = cb.y2;

            agg::trans_affine mtx1;
            double scale = 0.2;
            mtx1 *= agg::trans_affine_translation((m_min_x + m_max_x) * -0.5, (m_min_y + m_max_y) * -0.5);
            mtx1 *= agg::trans_affine_scaling(scale);
            int start = (m_min_x + m_max_x) * 0.5 * scale + rc.x1;
            mtx1 *= agg::trans_affine_translation(start, (m_min_y + m_max_y) * 0.5 + rc.y1 + 30);
            
            pod_ctrl[i]->render(ras, sl, ren, mtx1, rc, opacity);

        }

        //m_path.render(ras, sl, ren, mtx, cb, opacity);
    }
protected:
private:
    int gap;
};

class Button : public SvgCtrl
{
public:
    Button(const char* path):SvgCtrl(path){}
    virtual void on_click()
    {

    }
protected:
private:
};

#endif