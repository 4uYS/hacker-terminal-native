# 文档编辑器技能 (Document Editor)

## 技能描述
提供各种文档处理功能，支持多种格式的创建、编辑、转换和统计。

## 权限要求
- Level 2 及以上

## 支持的格式
- 纯文本 (.txt)
- Markdown (.md)
- HTML (.html, .htm)
- CSV (.csv)
- JSON (.json)

## 可用命令

### 1. 创建文档
**命令**: `create <文件路径>`

**说明**: 创建新的空白文档。

**参数**:
- `文件路径`: 要创建的文件路径

**选项**:
- `--template <模板名>: 使用指定模板
- `--type <类型>: 指定文档类型

**示例**:
```
create notes.txt
create report.md --template report
create data.csv
```

---

### 2. 编辑文档
**命令**: `edit <文件路径>`

**说明**: 打开文档进行编辑。

**参数**:
- `文件路径`: 要编辑的文件

**编辑模式**:
- 追加模式: 在文档末尾添加内容
- 插入模式: 在指定行插入内容
- 替换模式: 替换指定内容

**示例**:
```
edit readme.md
edit config.json
```

---

### 3. 查看文档
**命令**: `view <文件路径>`

**说明**: 查看文档内容，不修改。

**参数**:
- `文件路径`: 要查看的文件

**选项**:
- `-n`: 显示行号
- `-l <行数>: 只显示前 N 行
- `--head <行数>: 显示文件头部
- `--tail <行数>: 显示文件尾部

**示例**:
```
view document.txt
view -n source_code.c
view --tail 20 log.txt
```

---

### 4. 文档统计
**命令**: `stats <文件路径>`

**说明**: 统计文档的各项指标。

**参数**:
- `文件路径`: 要统计的文件

**统计内容**:
- 字符数
- 单词数
- 行数
- 段落数
- 字节大小
- 编码格式

**示例**:
```
stats essay.txt
stats report.md
```

---

### 5. 格式转换
**命令**: `convert <源文件> <目标文件>`

**说明**: 将文档从一种格式转换为另一种格式。

**参数**:
- `源文件`: 原始文件路径
- `目标文件`: 目标文件路径（扩展名决定格式）

**支持的转换**:
- Markdown → HTML
- HTML → 纯文本
- CSV → JSON
- JSON → CSV
- 纯文本 → Markdown

**示例**:
```
convert readme.md readme.html
convert data.csv data.json
convert page.html page.txt
```

---

### 6. 合并文档
**命令**: `merge <输出文件> <文件1> <文件2> ...`

**说明**: 将多个文档合并为一个。

**参数**:
- `输出文件`: 合并后的输出文件
- `文件1, 文件2...`: 要合并的文件列表

**选项**:
- `--separator <分隔符>: 文件之间的分隔符
- `--add-filenames`: 在每个文件前添加文件名

**示例**:
```
merge full_report.md part1.md part2.md part3.md
merge all.txt file1.txt file2.txt --separator "---"
```

---

### 7. 搜索替换
**命令**: `replace <文件> <查找> <替换>`

**说明**: 在文档中批量替换文本。

**参数**:
- `文件`: 目标文件
- `查找`: 要查找的文本
- `替换`: 替换后的文本

**选项**:
- `--regex`: 使用正则表达式
- `--case-insensitive`: 忽略大小写
- `--dry-run`: 预览替换结果，不实际修改

**示例**:
```
replace document.txt old_text new_text
replace config.json localhost 127.0.0.1
replace code.py --regex "def\s+\w+" "function"
```

---

### 8. 文档预览
**命令**: `preview <文件>`

**说明**: 预览文档的渲染效果。

**参数**:
- `文件`: 要预览的文件

**支持的预览**:
- Markdown: 渲染为格式化文本
- HTML: 纯文本预览
- JSON: 格式化输出
- CSV: 表格形式显示

**示例**:
```
preview readme.md
preview data.json
preview table.csv
```

---

### 9. 字数统计（中文）
**命令**: `wordcount <文件>`

**说明**: 专门针对中文文档的字数统计。

**参数**:
- `文件`: 要统计的文件

**统计内容**:
- 总字数（含中文）
- 中文字符数
- 英文单词数
- 数字字符数
- 标点符号数
- 空格和换行

**示例**:
```
wordcount article.txt
wordcount 报告.md
```

## 模板系统

### 内置模板
1. **空白模板** - 空文档
2. **报告模板** - 包含标题、目录、章节结构
3. **会议纪要模板** - 会议记录格式
4. **待办清单模板** - TODO 列表格式
5. **日记模板** - 每日记录格式

### 使用模板
```
create report.md --template report
create meeting_notes.md --template meeting
```

## 注意事项
1. 编辑操作会直接修改原文件，建议先备份
2. 大文件处理可能需要较长时间
3. 格式转换可能会丢失部分格式信息
4. 正则表达式替换请谨慎使用，建议先用 --dry-run 预览
5. 中文统计以 UTF-8 编码为准
