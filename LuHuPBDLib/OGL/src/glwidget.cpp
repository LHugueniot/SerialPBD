#include "glwidget.h"
#include <GL/glut.h>
#include <QKeyEvent>

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer.start(1);
    setFocusPolicy(Qt::StrongFocus);
    CamPos={10,10,-10};
    LookAt={0,0,0};
}

void GLWidget::initializeGL()
{
    glClearColor(1,1,1,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    m_solver= new LuHu::solver(0.99, glm::vec3(0,-0.01,0));
    //addPlain();
    //addCone();
    //addcube();
    BendingConTest();
}

void GLWidget::paintGL()
{
    glLoadIdentity();
    gluLookAt(CamPos[0],CamPos[1],CamPos[2], LookAt[0], LookAt[1], LookAt[2], 0, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(1, 0, 0.5);

    if(simulate==true && m_solver)
    {
        m_solver->RunSolver(5);

    }

    glRotatef(angle,0,1,0);
    drawPBDObjects(LINES);

    glLoadIdentity();
    //angle++;

}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w/h, 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(CamPos[0],CamPos[1],CamPos[2], LookAt[0], LookAt[1], LookAt[2], 0, 1, 0);
}

//-------------------------------------------------------Camera manipulation--------------------------------------------

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    makeCurrent();
    switch(event->key()){

    case Qt::Key_Escape:
        std::cout<<"Esc pressed";
        break;
    case Qt::Key_W:
        CamPos[0]+=1;
        LookAt[0]+=1;
        break;
    case Qt::Key_S:
        CamPos[0]-=1;
        LookAt[0]-=1;
        break;
    case Qt::Key_A:
        angle-=2.0;
        break;
    case Qt::Key_D:
        angle+=2.0;
        break;
    case Qt::Key_M:
        simulate=!simulate;
        break;
    case Qt::Key_N:
        //simulate=true;
        break;
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        std::cout<<"Esc unpressed";
    }
}

void GLWidget::BendingConTest()
{
    auto testObj = std::shared_ptr<LuHu::PBDobject>(new LuHu::PBDobject);
    auto p1 = std::shared_ptr<LuHu::point>(new LuHu::point(glm::vec3(0, 0, 0), glm::vec3(0), 1.0f));
    auto p2 = std::shared_ptr<LuHu::point>(new LuHu::point(glm::vec3(0, 1, 0), glm::vec3(0), 1.0f));
    auto p3 = std::shared_ptr<LuHu::point>(new LuHu::point(glm::vec3(0, 2, 0), glm::vec3(0), 1.0f));

    auto p4 = std::shared_ptr<LuHu::point>(new LuHu::point(glm::vec3(1, 0, 0), glm::vec3(0), 1.0f));
    auto p5 = std::shared_ptr<LuHu::point>(new LuHu::point(glm::vec3(1, 1, 0), glm::vec3(0), 1.0f));
    auto p6 = std::shared_ptr<LuHu::point>(new LuHu::point(glm::vec3(1, 2, 0), glm::vec3(0), 1.0f));

    auto p7 = std::shared_ptr<LuHu::point>(new LuHu::point(glm::vec3(1, 0, 1), glm::vec3(0), 1.0f));
    auto p8 = std::shared_ptr<LuHu::point>(new LuHu::point(glm::vec3(1, 1, 1), glm::vec3(0), 1.0f));
    auto p9 = std::shared_ptr<LuHu::point>(new LuHu::point(glm::vec3(1, 2, 1), glm::vec3(0), 1.0f));

    std::vector<std::shared_ptr<LuHu::point>> PointsVec{p1, p2, p3, p4, p5, p6, p7, p8, p9};

    auto bendConTest = std::shared_ptr<LuHu::bendingConstraint>  (new LuHu::bendingConstraint(p1, p2, p3,0));

    auto distDonTest12 = std::shared_ptr<LuHu::distanceConstraint>(new LuHu::distanceConstraint(p1,p2));


    p1->setIM(0);
    p3->setIM(0);

    std::vector<std::shared_ptr<LuHu::constraint>> ConsVec{
                std::shared_ptr<LuHu::distanceConstraint>(new LuHu::distanceConstraint(p1,p2)),
                std::shared_ptr<LuHu::distanceConstraint>(new LuHu::distanceConstraint(p2,p3)),
                std::shared_ptr<LuHu::distanceConstraint>(new LuHu::distanceConstraint(p1,p4)),
                std::shared_ptr<LuHu::distanceConstraint>(new LuHu::distanceConstraint(p2,p4)),
                std::shared_ptr<LuHu::distanceConstraint>(new LuHu::distanceConstraint(p2,p5)),
                std::shared_ptr<LuHu::distanceConstraint>(new LuHu::distanceConstraint(p3,p5)),
                std::shared_ptr<LuHu::distanceConstraint>(new LuHu::distanceConstraint(p3,p6)),
                std::shared_ptr<LuHu::distanceConstraint>(new LuHu::distanceConstraint(p4,p5)),
                std::shared_ptr<LuHu::distanceConstraint>(new LuHu::distanceConstraint(p5,p6)),
                std::shared_ptr<LuHu::distanceConstraint>(new LuHu::distanceConstraint(p4,p7)),
                std::shared_ptr<LuHu::distanceConstraint>(new LuHu::distanceConstraint(p5,p7)),
                std::shared_ptr<LuHu::distanceConstraint>(new LuHu::distanceConstraint(p5,p8)),
                std::shared_ptr<LuHu::distanceConstraint>(new LuHu::distanceConstraint(p6,p8)),
                std::shared_ptr<LuHu::distanceConstraint>(new LuHu::distanceConstraint(p6,p9)),
                std::shared_ptr<LuHu::distanceConstraint>(new LuHu::distanceConstraint(p7,p8)),
                std::shared_ptr<LuHu::distanceConstraint>(new LuHu::distanceConstraint(p8,p9)),

                std::shared_ptr<LuHu::bendingConstraint>  (new LuHu::bendingConstraint(p1, p4, p7,0)),
                std::shared_ptr<LuHu::bendingConstraint>  (new LuHu::bendingConstraint(p2, p5, p8,0)),
                std::shared_ptr<LuHu::bendingConstraint>  (new LuHu::bendingConstraint(p3, p6, p9,0)),

                std::shared_ptr<LuHu::bendingConstraint>  (new LuHu::bendingConstraint(p1, p2, p3,0)),
                std::shared_ptr<LuHu::bendingConstraint>  (new LuHu::bendingConstraint(p4, p5, p6,0)),
                std::shared_ptr<LuHu::bendingConstraint>  (new LuHu::bendingConstraint(p7, p8, p9,0))};

    testObj->Initialize(glm::vec3(0,1,0), PointsVec);

    testObj->addConstraint(ConsVec);

    //testObj->addPoint(p1);
    //testObj->addPoint(p2);
    //testObj->addPoint(p3);
    //testObj->addPoint(p4);

    m_solver->addPBDobject(testObj);
}

void GLWidget::addPlain()
{
    auto m_testObj = std::shared_ptr<LuHu::PBDobject>(new LuHu::PBDobject);
    auto a=m_testObj->Initialize("/home/s4906706/Documents/AP/Projects/APproj/LuHuPBDLib/PBDLib/models/plaine.obj",0,glm::vec3(0,5,0));
    if(a)
    {
        std::cout<<"working!";

        m_solver->addPBDobject(m_testObj);

        auto p1 = m_testObj->getPoints()[0];
        auto p2 = m_testObj->getPoints()[21];
        auto p3 = m_testObj->getPoints()[31];

        auto bendConTest = std::shared_ptr<LuHu::bendingConstraint>  (new LuHu::bendingConstraint(p1, p2, p3,1));
        m_testObj->addConstraint(bendConTest);

        //m_testObj->getPoints()[0]->setIM(0);
      //          for(uint i=0 ;i<121; i+=7)
      //          m_testObj->getPoints()[i]->setIM(0);

        m_testObj->getPoints()[20]->setIM(0);
//        m_testObj->getPoints()[21]->setIM(0);

        m_testObj->getPoints()[0]->setIM(0);
//        m_testObj->getPoints()[22]->setIM(0);

        //        m_testObj->getPoints()[23]->setIM(0);
        //        m_testObj->getPoints()[34]->setIM(0);
    }
    else
    {
        std::cout<<"its not working";
    }
}

void GLWidget::addcube()
{
    auto m_testObj = std::shared_ptr<LuHu::PBDobject>(new LuHu::PBDobject);
    auto a=m_testObj->Initialize("/home/s4906706/Documents/AP/Projects/APproj/LuHuPBDLib/PBDLib/models/deCube.obj",0,glm::vec3(0,5,0));
    if(a)
    {
        m_solver->addPBDobject(m_testObj);
        m_testObj->getPoints()[0]->setIM(0);
    }
    else
    {
        std::cout<<"its not working";
    }
}

void GLWidget::addCone()
{
    auto m_testObj = std::shared_ptr<LuHu::PBDobject>(new LuHu::PBDobject);
    auto a=m_testObj->Initialize("/home/s4906706/Documents/AP/Projects/APproj/LuHuPBDLib/PBDLib/models/cone.obj",0,glm::vec3(0));

    if(a)
    {
        auto testobj =m_testObj->getPoints();

        std::shared_ptr<LuHu::point> topPoint;
        for(auto p : testobj)
        {
            if(p.get()->getP().y>0.1)
            {
                topPoint=p;

            }
        }
        std::shared_ptr<LuHu::point> newp(new LuHu::point(glm::vec3(0,5,0), glm::vec3(0,0,0), 1.0));

        newp->setIM(0);
        std::shared_ptr<LuHu::distanceConstraint> newCon(new LuHu::distanceConstraint(newp,topPoint));

        newCon->setRestLength(5);

        m_solver->addPBDobject(m_testObj);

        m_testObj->addPoint(newp);
        m_testObj->addConstraint(newCon);
    }
    else
    {
        std::cout<<"its not working";
    }
}

bool GLWidget::drawPBDObjects(paintType _type)
{
    uint totalNumOfObj = m_solver->getObjects().size();

    if(totalNumOfObj!=0)
    {
        for(auto o : m_solver->getObjects())
        {
            switch (_type) {
            case LINES:
            {
                glBegin(GL_LINES);


                for(uint i=0; i<o->getConstraints().size(); i++)
                {

                    auto p1 = o->getConstraints()[i]->getPoint(0)->getP();
                    auto w1 = o->getConstraints()[i]->getPoint(0)->getIM();

                    auto p2 = o->getConstraints()[i]->getPoint(1)->getP();
                    auto w2 = o->getConstraints()[i]->getPoint(1)->getIM();

                    if(w1== 0 )
                        glColor3f(0, 0, 1);
                    else
                        glColor3f(1, 0, 0);

                    glVertex3f(p1.x, p1.y, p1.z);

                    if(w2== 0 )
                        glColor3f(0, 0, 1);
                    else
                        glColor3f(0, 1, 0);

                    glVertex3f(p2.x, p2.y, p2.z);
                }

                glEnd();
                break;
            }
            case TRIANGLES:
            {
                glBegin(GL_TRIANGLES);

                for(uint i=0; i<o.get()->getFacesPoints().size()-1; i+=3)
                {
                    auto p1=o->getFacesPoints()[i]->getP();
                    auto p2=o->getFacesPoints()[i+1]->getP();
                    auto p3=o->getFacesPoints()[i+2]->getP();

                    auto w1 = o->getFacesPoints()[i]->getIM();
                    auto w2 = o->getFacesPoints()[i+1]->getIM();
                    auto w3 = o->getFacesPoints()[i +2]->getIM();

                    if(w1== 0 )
                        glColor3f(0, 0, 1);
                    else
                        glColor3f(1, 0, 0);

                    glVertex3f(p1.x, p1.y, p1.z);

                    if(w2== 0 )
                        glColor3f(0, 0, 1);
                    else
                        glColor3f(1, 0, 0);

                    glVertex3f(p2.x, p2.y, p2.z);

                    if(w3== 0 )
                        glColor3f(0, 0, 1);
                    else
                        glColor3f(1, 0, 0);

                    glVertex3f(p3.x, p3.y, p3.z);

                }
                glEnd();
                break;
            }
            default:
                break;
            }

        }
    }
}

void GLWidget::drawGrid(uint size)
{
    glBegin(GL_LINES);

    glEnd();
}
