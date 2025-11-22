#include "haptic.h"
#include <QDebug>  // 必须加这一行
#include <qcoreapplication_platform.h>
#include <qstringliteral.h>

#if defined(Q_OS_ANDROID)
#include <QJniObject>
#include <QJniEnvironment>

void Haptic::vibrate(int ms)
{
    // 获取当前 Android Context
    auto ctx = QNativeInterface::QAndroidApplication::context();
    if (!ctx.isValid()) {
        qWarning() << "INVALID CONTEXT. IS THE APP NAME CORRECT?";
        return;
    }

    // 包装成 QJniObject
    QJniObject context(ctx);

    // 获取 Vibrator 系统服务
    QJniObject vibrator = context.callObjectMethod(
        "getSystemService",
        "(Ljava/lang/String;)Ljava/lang/Object;",
        QJniObject::fromString("vibrator").object()
        );

    if (!vibrator.isValid())
        return;

    // 调用 Vibrator.vibrate(long)
    vibrator.callMethod<void>("vibrate", "(J)V", static_cast<jlong>(ms));
}
#elif defined(Q_OS_IOS)
#include <AudioToolbox/AudioServices.h>
void Haptic::vibrate(int) {
    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
}
#else
void Haptic::vibrate(int) { /* Desktop do nothing */ }
#endif
