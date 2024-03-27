#ifndef PRINTABLE_H
#define PRINTABLE_H
#define PRINTABLE_CLASS static const std::string& CLASS_NAME; PRINTABLE_CLASS_FRIENDSHIP
#define PRINTABLE_CLASS_NAME(name) const std::string& name::CLASS_NAME = #name;
#define PRINTABLE_CLASS_FRIENDSHIP friend class WorkspacePrinter; friend class CompilePrinter;
#define PRINTABLE_DECLARE_PRINT(t) virtual void print(Printer* const printer) const{printer->print##t(this);}
#endif // PRINTABLE_H
