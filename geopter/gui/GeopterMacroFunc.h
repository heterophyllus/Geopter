#ifndef GEOPTERMACROFUNC_H
#define GEOPTERMACROFUNC_H

/**
 *  This file describes the declarations of geopter macro functions.
 *  Implementations are written in GeopterMacroFunc.cpp
 *
 *  DECRARE_GEOPTER_MACRO_FUNC should be called inside of MainWindow class declaration.
 *
 */

#define DECRARE_GEOPTER_MACRO_FUNC  \
public Q_SLOTS:                     \
    QString app_dir();  \
    void add_field(double x, double y, double wt=1.0, QColor color= Qt::black, double vux=0.0, double vlx=0.0, double vuy=0.0, double vly=0.0); \
    void add_wavelength(double wl, double wt= 1.0, QColor color= Qt::black); \
    void chromatic_focus_shift();  \
    void delete_surface(int s);     \
    void draw2d();  \
    void field_curvature();       \
    QString glass(int s);          \
    double indx(); /** Index of refraction at the primary wavelength */ \
    void insert_surface(int s);      \
    void load_lens(QString filename); \
    void longitudinal(); \
    void new_lens(); \
    int  nfld(); /** Returns number of fields */           \
    int  nsur(); /** Returns number of surfaces */         \
    int  nwav(); /** Returns number of wavelengths */      \
    void paraxial_trace(); \
    void prescription(); \
    int  pwav(); /** Returns primary wavelength index */   \
    void raytrace(double px, double py, int fi, int wi=-1); \
    void save_lens(QString filename); \
    void set_field_type(int type); \
    void set_note(QString note); \
    void set_pupil_type(int type); \
    void set_pupil_value(double value); \
    void set_surface_glas(int s, QString glassname); /** Set glass of the surface */  \
    void set_surface_radi(int s, double r); /** Set radius of the surface */     \
    void set_surface_thic(int s, double t); /** Set thickness of the surface */  \
    void set_title(QString title); \
    void spot_diagram(); \
    int  stop_surf(); /** Returns stop surface index */    \
    double radi(int s);  \
    double thic(int s);  \
    void transverse(); \
    void update_ui(); \
private:   \
    bool check_surface_range(int s); \
    Rgb QColorToRgb(const QColor& color);


#endif //GEOPTERMACROFUNC_H
