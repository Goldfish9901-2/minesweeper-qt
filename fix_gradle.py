import os

def main(
        wrapper:str = r'build/Android_Qt_6_8_5_Clang_arm64_v8a-Debug/android-build-MineSweeper/gradle/wrapper/gradle-wrapper.properties',
        key:str="distributionUrl",
        value:str=r'https://mirrors.tuna.tsinghua.edu.cn/gradle/gradle-8.10-all.zip'
):
    if not os.path.exists(wrapper):
        print(f"{wrapper} not exists.")
        return
    outbuf=[]
    # text = open(wrapper).read()
    # text = text.replace(
    #     'https://services.gradle.org/distributions/gradle-8.10-bin.zip',
    #     
    # )
    # open(wrapper, 'w').write(text)
    for line in open(wrapper):
        header,*content=line.split("=")
        if header!=key:outbuf.append(line.strip('\n'))
        else:
            outbuf.append(f"{key}={value}")
            print(f"patched {key} to {value}")
    with open(wrapper, 'w') as f:
        f.write("\n".join(outbuf))
        print("patched.")

