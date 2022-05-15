#include "semantics.hpp"

void semanticAnalyser::analyseTree(treeNode* t){
    if(t==nullptr) return;
    int enter=0;
    switch (t->getNodeKind())
    {
    case DK_Func:
        st.addSymbol(t, DK_Func);
        st.enterScope(); enter=1;
        st.addSymbol(t, DK_Var);
        if(t->getChild().size()>2){
            treeNode* para=t->getChild().at(1);
            if(para->getNodeKind()!=TK_Id){
                while (para!=nullptr)
                {
                    st.addSymbol(para, DK_Var);
                    para=para->getSibling();
                }
            }
        }
        break;
    case DK_Const:
    case DK_Var:
        st.addSymbol(t, DK_Var);
        break;
    case DK_Proc:
        st.addSymbol(t, DK_Proc);
        st.enterScope(); enter=1;
        if(t->getChild().size()>2){
            treeNode* para=t->getChild().at(1);
            if(para->getNodeKind()!=TK_Id){
                while (para!=nullptr)
                {
                    st.addSymbol(para, DK_Var);
                    para=para->getSibling();
                }
            }            
        }
        break;        
    case DK_Type:
        st.addSymbol(t, DK_Type);
        break;
    case SK_Assign:
        if(t->getChild().size()>0 && t->getChild().at(0)->getNodeKind()==TK_Id){
            check_decl(t->getChild().at(0)->getIdName(), DK_Var, t->getLine());
        }    
        break;    
    default:
        break;
    }
    for(int i=0; i<t->getChild().size(); i++){
        analyseTree(t->getChild().at(i));
    }
    if(t->getSibling()!=nullptr){
        analyseTree(t->getSibling());
    }
    if(enter==1) st.leaveScope();
}
void semanticAnalyser::check_decl(string name, int kind, int line){
    TableItem* item=st.findSymbol(name, kind);
    if(item==nullptr){
        // cout<<"UNDEFINED"<<endl;
        err.add(UNDEFINED, name, line);
    }
}