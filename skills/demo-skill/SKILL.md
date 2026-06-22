# Demo Skill - 示例技能

## 技能名称
demo-skill

## 技能描述
这是一个示例技能，用于演示技能系统的使用方法。

## 功能
- 演示技能文档格式
- 演示技能调用方式
- 提供技能开发模板

## 使用方法

### 查看技能文档
```
/read-skill demo-skill
```

### 执行技能
```
/run-skill demo-skill 参数1 参数2
```

## 参数说明

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| name | string | 否 | 名称 |
| action | string | 否 | 动作 |

## 返回值
字符串类型的执行结果。

## 示例

### 示例1：基本使用
```
/run-skill demo-skill name=测试
```

输出：
```
执行技能: demo-skill
参数: name=测试
```

## 注意事项
- 这是一个演示技能，没有实际功能
- 实际技能需要编译 skill.dll 才能执行
- 技能文档使用 Markdown 格式

## 版本
v1.0.0

## 作者
爱折腾实验室 · 豆包 AI Agent Lab
