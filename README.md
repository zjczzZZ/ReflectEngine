引擎只提供类似游戏逻辑tick框架的功能，并无渲染

一、侵入式反射引擎，提供entity与component父类，以组合模式构建实体与功能组件。


二、可将注册反射功能的实体与组件的组合模式序列化与反序列化


三、框架为主线程与线程池同步

加载2个标准测试实体时，加速比为460倍以上

加载100个标准测试实体时，加速比为200倍以上

![v](https://github.com/zjczzZZ/ReflectEngine/assets/167063511/6197a0a4-4190-4387-92a9-5a177121f05e)

![perf](https://github.com/zjczzZZ/ReflectEngine/assets/167063511/f0700f34-2b42-4542-8baf-aa6e9dcb4e0c)
