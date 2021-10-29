#ifndef GEOPTER_QT_BINDING_H
#define GEOPTER_QT_BINDING_H

/**
 * Header file to bind geopter optical module and Qt through PythonQt
 * 
 * In order to call geopter object such as OpticalSystem from python console, 
 * these classes inherits QObject.  All of the object must be added to main context.
 * 
 */
#if BIND_PYTHONQT
    #include <QObject>

    #ifndef INHERIT_QOBJECT
        #define INHERIT_QOBJECT \
            :public QObject
    #endif

    #ifndef DECL_QOBJECT_QSLOTS
        #define DECL_QOBJECT_QSLOTS \
            Q_OBJECT \
            public Q_SLOTS:
    #endif

    #ifndef INIT_QOBJECT
    #define INIT_QOBJECT \
            :QObject(NULL)
    #endif

#else
    #ifndef INHERIT_QOBJECT
        #define INHERIT_QOBJECT
    #endif

    #ifndef DECL_QOBJECT_QSLOTS
        #define DECL_QOBJECT_QSLOTS \
            public:
    #endif

    #ifndef INIT_QOBJECT
        #define INIT_QOBJECT
    #endif
#endif


#endif //GEOPTER_QT_BINDING_H
