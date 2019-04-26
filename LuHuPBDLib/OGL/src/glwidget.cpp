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
    glClearColor(0.75,0.75,0.75,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    m_solver= new LuHu::solver(0.95f, glm::vec3(0,-0.2,0));
    //addSmolPlain();
    addPlain();
    //addCone();
    //addcube();
    //BendingConTest();
    time=0;
}

void GLWidget::paintGL()
{
    glLoadIdentity();
    gluLookAt(CamPos[0],CamPos[1],CamPos[2], LookAt[0], LookAt[1], LookAt[2], 0, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(1, 0, 0.5);

    if(simulate==true && m_solver)// && time%10==0)
    {
        m_solver->RunSolver(5,5);
    }
    glRotatef(angley,1,0,1);
    glRotatef(anglex,0,1,0);
    drawPBDObjects(LINES);
    drawGrid(10);
    glLoadIdentity();
    //angle++;
    time++;

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
        angley-=2.0;
        paintGL();
        updateGL();
        break;
    case Qt::Key_S:
        angley+=2.0;
        paintGL();
        updateGL();
        break;
    case Qt::Key_A:
        anglex+=1.0;
        paintGL();
        updateGL();
        break;
    case Qt::Key_D:
        anglex-=1.0;
        paintGL();
        updateGL();
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

    //testObj->Initialize()
    //testObj->Initialize(glm::vec3(0,1,0), PointsVec);


    //testObj->addPoint(p1);
    //testObj->addPoint(p2);
    //testObj->addPoint(p3);
    //testObj->addPoint(p4);

    m_solver->addPBDobject(testObj);
}

void GLWidget::addPlain()
{
    auto TestObj = std::shared_ptr<LuHu::PBDobject>(new LuHu::PBDobject);
    auto a=TestObj->Initialize("plaine.obj",0,glm::vec3(0,5,0),1);
    if(a)
    {
        std::cout<<"working!";

        m_solver->addPBDobject(TestObj);
        TestObj->setPointInvMass(0,0);
        TestObj->setPointInvMass(20,0);
        TestObj->addBendingConstraints(LuHu::generateBendConstraints(TestObj->getPointPositions(), TestObj->getDistanceConstraints()));

        //        for(TestObj.get)
        //        {

        //        }
        std::cout<<"Bending cons size is: "<<TestObj->getBendingConstraints().size()<<"\n";
    }
    else
    {
        std::cout<<"its not working";
    }
}
void GLWidget::addSmolPlain()
{
    auto TestObj = std::shared_ptr<LuHu::PBDobject>(new LuHu::PBDobject);

    std::vector<glm::vec3> positions{
        glm::vec3(0),
                glm::vec3(0,0,1),
                glm::vec3(0,0,2),
                glm::vec3(1,0,0),
                glm::vec3(1,0,1),
                glm::vec3(1,0,2),
                glm::vec3(2,0,0),
                glm::vec3(2,0,1),
                glm::vec3(2,0,2),
    };
    std::vector<glm::vec3> velocities{
        glm::vec3(0),
                glm::vec3(0),
                glm::vec3(0),
                glm::vec3(0),
                glm::vec3(0),
                glm::vec3(0),
                glm::vec3(0),
                glm::vec3(0),
                glm::vec3(0),

    };

    std::vector<float> masses{1,1,1,1,1,1,1,1,1};
    TestObj->Initialize(glm::vec3(0,3,0),positions, velocities, masses);

    std::vector<uint> ditConsraints{0,1,
                                    1,2,
                                    0,3,
                                    0,4,
                                    1,4,
                                    1,5,
                                    2,5,
                                    3,4,
                                    4,5,
                                    3,6,
                                    3,7,
                                    4,7,
                                    4,8,
                                    6,7,
                                    7,8,
                                    5,8
                                   };

    //    std::vector<uint> bendConstraints{0,3,6,
    //                                     1,4,7,
    //                                     2,5,8,
    //                                     3,4,5,
    //                                     6,7,8};

    TestObj->addBendingConstraints(LuHu::generateBendConstraints(TestObj->getPointPositions(), TestObj->getDistanceConstraints()));
    TestObj->addDistConstraints(ditConsraints);
    //TestObj->addBendingConstraints(bendConstraints);
    TestObj->setPointInvMass(0,0);
    TestObj->setPointInvMass(6,0);

    m_solver->addPBDobject(TestObj);

}
void GLWidget::addcube()
{
    std::shared_ptr<LuHu::PBDobject> TestObj(new LuHu::PBDobject);

    auto a=TestObj->Initialize("deCube.obj",0,glm::vec3(0,5,0),1);

    if(a)
    {
        m_solver->addPBDobject(TestObj);

        TestObj->setPointInvMass(0,0);
        TestObj->addBendingConstraints(LuHu::generateBendConstraints(TestObj->getPointPositions(), TestObj->getDistanceConstraints()));
    }
    else
    {
        std::cout<<"its not working";
    }
}

void GLWidget::addCone()
{
    //    auto m_testObj = std::shared_ptr<LuHu::PBDobject>(new LuHu::PBDobject);
    //    auto a=m_testObj->Initialize("/home/s4906706/Documents/AP/Projects/APproj/LuHuPBDLib/PBDLib/models/cone.obj",0,glm::vec3(0));

    //    if(a)
    //    {
    //        auto testobj =m_testObj->getPoints();

    //        std::shared_ptr<LuHu::point> topPoint;
    //        for(auto p : testobj)
    //        {
    //            if(p.get()->getP().y>0.1)
    //            {
    //                topPoint=p;

    //            }
    //        }
    //        std::shared_ptr<LuHu::point> newp(new LuHu::point(glm::vec3(0,5,0), glm::vec3(0,0,0), 1.0));

    //        newp->setIM(0);
    //        std::shared_ptr<LuHu::distanceConstraint> newCon(new LuHu::distanceConstraint(newp,topPoint));

    //        newCon->setRestLength(5);

    //        m_solver->addPBDobject(m_testObj);

    //        m_testObj->addPoint(newp);
    //        m_testObj->addConstraint(newCon);
    //    }
    //    else
    //    {
    //        std::cout<<"its not working";
    //    }
}

bool GLWidget::drawPBDObjects(paintType _type)
{
    uint totalNumOfObj = m_solver->getObjects().size();

    if(totalNumOfObj!=0)
    {
        for(auto o : m_solver->getObjects())
        {
            switch (_type)
            {
            case LINES:
            {
                glBegin(GL_LINES);


                for(uint i=0; i<o->getDistanceConstraints().size(); i+=2)
                {
                    uint ConIndex1=o->getDistanceConstraints()[i];
                    uint ConIndex2=o->getDistanceConstraints()[i+1];

                    auto p1 = o->getPointPosition(ConIndex1);
                    auto p2 = o->getPointPosition(ConIndex2);

                    auto w1 = o->getPointInvMass(ConIndex1);
                    auto w2 = o->getPointInvMass(ConIndex2);

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

                for(uint i=0; i<o->getFacesPoints().size()-1; i+=3)
                {

                    auto index1=o->getFacesPoints()[i];
                    auto index2=o->getFacesPoints()[i+1];
                    auto index3=o->getFacesPoints()[i+2];

                    auto p1=o->getPointPosition(index1);
                    auto p2=o->getPointPosition(index2);
                    auto p3=o->getPointPosition(index3);

                    auto w1 = o->getFacesPoints()[index1];
                    auto w2 = o->getFacesPoints()[index2];
                    auto w3 = o->getFacesPoints()[index3];

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
            }

        }
    }
}

void GLWidget::drawGrid(uint size)
{

    glBegin(GL_LINES);

    glLineWidth(3);
    glColor3f(1,0,0); //x red
    glVertex3f(0,0.01f,0);
    glVertex3f(1,0.01f,0);

    glColor3f(0,1,0);// y green
    glVertex3f(0,0.01f,0);
    glVertex3f(0,1.01f,0);

    glColor3f(0,0,1);// z blue
    glVertex3f(0,0.01f,0);
    glVertex3f(0,0.01f,1);


    glLineWidth(1);
    glColor3f(0.6,0.6,0.6);
    float scale=0.5;
    for(int x=0; x<size+1; x++)
    {
        for(int z=0; z<size+1; z++)
        {
            float xx=x*scale-(size*scale)/2;
            float zz=z*scale-(size*scale)/2;

            if(x==(size+1)/2 || z==(size+1)/2 ){
                glColor3f(0.9,0.7,0.5);
            }
            else {
                glColor3f(0.6,0.6,0.6);

            }

            glVertex3f(xx,0,zz);

            glVertex3f(xx,0,(size*scale)/2);



            glVertex3f(zz,0,xx);

            glVertex3f((size*scale)/2,0,xx);

        }
    }
    glEnd();
}
