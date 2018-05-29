#ifndef SCRIPTINGENGINE_H
#define SCRIPTINGENGINE_H

#include <QObject>
#include <QMetaType>
#include <QMetaMethod>
#include <QDebug>
#include <QVariant>


#include "singelton.h"
#include "dukglue.h"
#include "ioc.h"
//#include "duktape/console/duk_console.h"
//#include "duktape/logging/duk_logging.h"
//#include "duktape/print-alert/duk_print_alert.h"


//duk_ret_t init_logging(duk_context *ctx, void *udata) {
//	(void)udata;
//	duk_logging_init(ctx, 0 /*flags*/);
//	//printf("top after init: %ld\n", (long)duk_get_top(ctx));
//
//	/* C API test */
//	duk_eval_string_noresult(ctx, "Duktape.Logger.clog.l = 0;");
//	duk_log(ctx, DUK_LOG_TRACE, "c logger test: %d", 123);
//	duk_log(ctx, DUK_LOG_DEBUG, "c logger test: %d", 123);
//	duk_log(ctx, DUK_LOG_INFO, "c logger test: %d", 123);
//	duk_log(ctx, DUK_LOG_WARN, "c logger test: %d", 123);
//	duk_log(ctx, DUK_LOG_ERROR, "c logger test: %d", 123);
//	duk_log(ctx, DUK_LOG_FATAL, "c logger test: %d", 123);
//	duk_log(ctx, -1, "negative level: %s %d 0x%08lx", "arg string", -123, 0xdeadbeefUL);
//	duk_log(ctx, 6, "level too large: %s %d 0x%08lx", "arg string", 123, 0x1234abcdUL);
//
//	return 0;
//}


//#define REGISTER_JS_CONSTRUCTOR(TYPE) \

//#define REGISTER_META_TYPE_JS_CLASS(TYPE) \
//    qRegisterMetaType<TYPE>(#TYPE);


class ScriptingEngine : public GenericSingleton<ScriptingEngine> , public IResolvable
{
    friend class GenericSingleton<ScriptingEngine>;

    ScriptingEngine() : IResolvable(Names::JSEngine::name()) {
        m_ctx = duk_create_heap_default();
        //duk_console_init(m_ctx, DUK_CONSOLE_PROXY_WRAPPER /*flags*/);
        //duk_logging_init(m_ctx, 0 /*flags*/);
        ////(void)duk_safe_call(m_ctx, init_logging, NULL, 0, 1);
        //duk_print_alert_init(m_ctx, 0 /*flags*/);
    }

public:



    duk_context * context() { return m_ctx; }

    ~ScriptingEngine(){
        duk_destroy_heap(m_ctx);
    }

    static duk_context * jsContext() {
        return ScriptingEngine::instance()->context();
    }

    template <class T>
    static void registerClass() {
        int metaTypeId = qRegisterMetaType<T*>();
        bool reg = QMetaType::isRegistered(metaTypeId);
        //const QMetaObject* metaObject = QMetaType::metaObjectForType(metaTypeId);
        const QMetaObject* metaObject = QMetaType::metaObjectForType(metaTypeId);
        if (metaObject == nullptr)  return;

        dukglue_register_delete<T>(ScriptingEngine::jsContext());

//        for (int i=0;i<metaObject->constructorCount();i++){
//            auto constructor = metaObject->constructor(i);
//            //qDebug() << metaObject->className()
//                     << " : " << constructor.methodSignature().toStdString() << "\n";

//            //qDebug() << "Argument list count :" << constructor.parameterCount()<<  "\n";
//            for(int j=0;j< constructor.parameterCount();j++)
//            {
//                QMetaType::Type atype  = (QMetaType::Type) constructor.parameterType(j);
//                switch (atype) {
//                        case QMetaType::Bool :  break;
//                        case QMetaType::Int :  break;
//                        case QMetaType::UInt :  break;
//                        case QMetaType::LongLong :  break;
//                        case QMetaType::ULongLong :  break;
//                        case QMetaType::Double :  break;
//                        case QMetaType::Long  :  break;
//                        case QMetaType::Short  :  break;
//                        case QMetaType::Char  :  break;
//                        case QMetaType::ULong  :  break;
//                        case QMetaType::UShort :  break;
//                        case QMetaType::UChar  :  break;
//                        case QMetaType::Float :  break;
//                        case QMetaType::VoidStar :  break;
//                        case QMetaType::QChar :  break;
//                        case QMetaType::QString  :  break;
//                        case QMetaType::QStringList  :  break;
//                        case QMetaType::QByteArray :  break;
//                        case QMetaType::QBitArray  :  break;
//                        case QMetaType::QDate  :  break;
//                        case QMetaType::QTime  :  break;
//                        case QMetaType::QDateTime  :  break;
//                        case QMetaType::QUrl :  break;
//                        case QMetaType::QLocale  :  break;
//                        case QMetaType::QRect  :  break;
//                        case QMetaType::QRectF  :  break;
//                        case QMetaType::QSize  :  break;
//                        case QMetaType::QSizeF :  break;
//                        case QMetaType::QLine  :  break;
//                        case QMetaType::QLineF  :  break;
//                        case QMetaType::QPoint  :  break;
//                        case QMetaType::QPointF  :  break;
//                        case QMetaType::QRegExp :  break;
//                        case QMetaType::QEasingCurve  :  break;
//                        case QMetaType::QUuid  :  break;
//                        case QMetaType::QVariant  :  break;
//                        case QMetaType::QModelIndex :  break;
//                        case QMetaType::QPersistentModelIndex  :  break;
//                        case QMetaType::QRegularExpression :  break;
//                        case QMetaType::QJsonValue  :  break;
//                        case QMetaType::QJsonObject  :  break;
//                        case QMetaType::QJsonArray  :  break;
//                        case QMetaType::QJsonDocument :  break;
//                        case QMetaType::QByteArrayList  :  break;
//                        case QMetaType::QObjectStar  :  break;
//                        case QMetaType::SChar :  break;
//                        case QMetaType::Void :  break;
//                        case QMetaType::User  :  break;
////                        case QMetaType::UnknownType :  break;
////                        case QMetaType::QVariantMap :  break;
////                        case QMetaType::QVariantList :  break;
////                        case QMetaType::QVariantHash :  break;
////                        case QMetaType::QFont  :  break;
////                        case QMetaType::QPixmap  :  break;
////                        case QMetaType::QBrush  :  break;
////                        case QMetaType::QColor  :  break;
////                        case QMetaType::QPalette :  break;
////                        case QMetaType::QIcon  :  break;
////                        case QMetaType::QImage  :  break;
////                        case QMetaType::QPolygon  :  break;
////                        case QMetaType::QRegion  :  break;
////                        case QMetaType::QBitmap :  break;
////                        case QMetaType::QCursor  :  break;
////                        case QMetaType::QKeySequence  :  break;
////                        case QMetaType::QPen  :  break;
////                        case QMetaType::QTextLength  :  break;
////                        case QMetaType::QTextFormat :  break;
////                        case QMetaType::QMatrix  :  break;
////                        case QMetaType::QTransform  :  break;
////                        case QMetaType::QMatrix4x4  :  break;
////                        case QMetaType::QVector2D :  break;
////                        case QMetaType::QVector3D  :  break;
////                        case QMetaType::QVector4D  :  break;
////                        case QMetaType::QQuaternion  :  break;
////                        case QMetaType::QPolygonF :  break;
////                        case QMetaType::QSizePolicy  :  break;
//                        default :  break;
//                 }

//            }

//        }


        dukglue_register_constructor<T>(ScriptingEngine::jsContext(), metaObject->className());
        for (int i = metaObject->methodOffset(); i < metaObject->methodCount(); ++i) {
            QMetaMethod method = metaObject->method(i);
            QString methodName = QString::fromLatin1(metaObject->method(i).name());
            if (method.methodType() == QMetaMethod::Slot &&
                    method.access() == QMetaMethod::Public) {
                // ARASH : LOOK HERE
                //dukglue_register_method<T>(ScriptingEngine::jsContext(), &T::jsSlotsProxy, methodName.toStdString().data());
                //qDebug() << methodName;
            }
        }
    }

    static bool eval(const QString & script, QString& out){
        bool success = true;
        out = "";
        if (duk_peval_string(ScriptingEngine::jsContext(), script.toStdString().data()) != 0) {
            duk_get_prop_string(ScriptingEngine::jsContext(), -1, "stack");
            out += QString("Error running '") + script + QString("\r\n");
            success = false;
        }
        else {
            out += QString::fromUtf8(duk_safe_to_string(ScriptingEngine::jsContext(), -1)) + QString("\r\n");
        }
        duk_pop(ScriptingEngine::jsContext());
        return success;
    }

private:
    duk_context * m_ctx = NULL;
};

template <class T>
class ScriptableObject   {
    
public:
    ScriptableObject(){
    }
    ~ScriptableObject(){
      
        dukglue_invalidate_object(ScriptingEngine::instance()->context(), this);
    }

    void jsSlotsProxy(){
        duk_push_this(ScriptingEngine::jsContext());
        duk_push_current_function(ScriptingEngine::jsContext());
        duk_get_prop_string(ScriptingEngine::jsContext(), -1, "name");
        const char * slotName = duk_require_string(ScriptingEngine::jsContext(), -1);
        try
        {
            T *  thisQObject = static_cast<T*>(this);
            if (thisQObject == nullptr)  return;
            QMetaObject::invokeMethod(thisQObject, slotName);
        }
        catch (const std::exception& e)
        {
            //qDebug() << "jsSlotProxy  Exception : " << e.what();
        }
    }
 
};

class JSTest : public QObject , public ScriptableObject<JSTest>{
    Q_OBJECT
public:
    JSTest() {
		//qDebug() << "i'm being constructed";
	}
    JSTest(const JSTest & /*copy*/) {}
    ~JSTest() {
		//qDebug() << "i'm being destructed";
	}

public slots:
    void slot1() { //qDebug() << "slot1: i'm being called"; 
	}
    void slot2() { //qDebug() << "slot2: i'm being called"; 
	}
};
//Q_DECLARE_METATYPE(JSTest*)
#endif // SCRIPTINGENGINE_H
