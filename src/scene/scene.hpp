#ifndef PACMAN_SCENE_HPP
#define PACMAN_SCENE_HPP

#include <QGraphicsScene>
#include <QAbstractGraphicsShapeItem>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QTimer>
#include <QMessageBox>

#include "figures_widget/figure.hpp"
#include "objects/wall.hpp"
#include "objects/pacman.hpp"

class Scene final : public QGraphicsScene {
    Q_OBJECT
public:
    explicit Scene(QObject* parent = nullptr)
    : QGraphicsScene(parent), timer_(this) {
        setSceneRect(0, 0, 250, 440);
        connect(&timer_, &QTimer::timeout, this, &Scene::MovePacman);
        connect(this, &Scene::GameStopped, this, &Scene::StopGame);
    }
signals:
    void GameStopped();
    void ScoreUpdated(uint);
public slots:
    void RemoveSelected() {
        for (auto&& item : selectedItems())
            removeItem(std::forward<decltype(item)>(item));
    }
    void AdjustWallsStretching(const box2& ds) {
        for(auto&& wall : std::ranges::views::filter(
            selectedItems(), SceneObject::TypeEquals(SceneObject::kWall)
        ))
            dynamic_cast<Wall*>(wall)->AdjustStretching(ds);
        update();
    }
    bool StartGame() {
        const auto itms = items();
        if (std::ranges::count_if(
            itms, SceneObject::TypeEquals(SceneObject::kWall)
        ) < minWallsAndCoins_) {
            QMessageBox::critical(
                (QWidget*)this->parent(), "Wrong game conditions",
                QString("Minimal walls amount allowed is %1!").arg(minWallsAndCoins_)
            );
            return false;
        }
        if (std::ranges::count_if(
            itms, SceneObject::TypeEquals(SceneObject::kCoin)
        ) < minWallsAndCoins_) {
            QMessageBox::critical(
                (QWidget*)this->parent(), "Wrong game conditions",
                QString("Minimal coins amount allowed is %1!").arg(minWallsAndCoins_)
            );
            return false;
        }
        if (std::ranges::count_if(
            itms, SceneObject::TypeEquals(SceneObject::kPacman)
        ) < 1) {
            QMessageBox::critical(
                (QWidget*)this->parent(), "Wrong game conditions",
                "You forgot to add pacman!"
            );
            return false;
        }
        for (auto&& item : itms) {
            std::forward<decltype(item)>(item)->setFlag(
                QAbstractGraphicsShapeItem::ItemIsMovable, false);
            std::forward<decltype(item)>(item)->setFlag(
                QAbstractGraphicsShapeItem::ItemIsSelectable, false);
        }
        timer_.start(100);
        return true;
    }
    void MovePacman() {
        if (Coin* coin = getPacman()->Move(); coin != nullptr) {
            removeItem(coin);
            emit ScoreUpdated(++score_);
            if (auto itms = items(); std::ranges::find_if(
                itms, SceneObject::TypeEquals(SceneObject::kCoin)
            ) == itms.end()) {
                QMessageBox::information(
                    (QWidget*)this->parent(), "You win",
                    "Congratulations, you have collected all coins!"
                );
                emit GameStopped();
            }
        }
    }
    void StopGame() {
        for (auto&& item : items()) {
            std::forward<decltype(item)>(item)->setFlag(
                QAbstractGraphicsShapeItem::ItemIsMovable, true);
            std::forward<decltype(item)>(item)->setFlag(
                QAbstractGraphicsShapeItem::ItemIsSelectable, true);
        }
        getPacman()->OpenMouth();
        timer_.stop();
        emit ScoreUpdated(score_ = 0);
    }
private:
    [[nodiscard]] Pacman* getPacman() const noexcept {
        auto itms = items();
        if (auto pacman = std::ranges::find_if(
            itms.begin(), itms.end(),
            SceneObject::TypeEquals(SceneObject::kPacman)
        ); pacman != itms.end())
            return dynamic_cast<Pacman*>(*pacman);
        return nullptr;
    }
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) final {
        if (event->mimeData()->hasFormat(Figure::kMimeFormat))
            event->acceptProposedAction();
        else
            event->ignore();
    }
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) final {
        if (event->mimeData()->hasFormat(Figure::kMimeFormat))
            event->accept();
    }
    void dropEvent(QGraphicsSceneDragDropEvent *event) final {
        if (event->mimeData()->hasFormat(Figure::kMimeFormat)) {
            QByteArray itemData = event->mimeData()->data(Figure::kMimeFormat);
            QDataStream dataStream(&itemData, QIODevice::ReadOnly);
            SceneObject::Type type;
            dataStream >> type;
            if (type != SceneObject::kPacman || std::ranges::none_of(
                items(), SceneObject::TypeEquals(SceneObject::kPacman)
            )) {
                SceneObject* item = SceneObject::Factory(type);
                item->setPos(event->scenePos());
                addItem(item);
            }
            event->acceptProposedAction();
        } else
            event->ignore();
    }
    void keyPressEvent(QKeyEvent* event) final {
        switch (event->key()) {
            case Qt::Key_Up:
                getPacman()->SetDirection(Pacman::kTop);
                break;
            case Qt::Key_Left:
                getPacman()->SetDirection(Pacman::kLeft);
                break;
            case Qt::Key_Down:
                getPacman()->SetDirection(Pacman::kBottom);
                break;
            case Qt::Key_Right:
                getPacman()->SetDirection(Pacman::kRight);
                break;
        }
    }
private:
    static constexpr uint minWallsAndCoins_ = 10;
    QTimer timer_;
    uint score_ = 0;
};

#endif
