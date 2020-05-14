import * as React from 'React';
import classnames from 'classnames';
import { Scene, SceneType } from './objects/Scene';
import { GriditemType, Griditem } from './objects/Griditem';
import { EmulatorState } from './objects/Emulator';
import { get } from 'https';

class Rect extends React.PureComponent<{
    x: number,
    y: number,
    height: number,
    width: number
    borderColor: string
}> {
    render() {
        return <div
            style={{
                left: this.props.x,
                top: this.props.y,
                height: this.props.height,
                width: this.props.width,
                border: 'solid ' + this.props.borderColor + ' 1px'
            }}
            className="object-rect"></div>
    }
}

function GetYpos(scene: SceneType, row: number, col: number) {
    let y;

    switch (scene) {
        case "roof":
        case "moon_night":
            y = (85 * row + (col < 5 ? 20 * (5 - col) : 0)) - 10;
            break;

        case "fog":
        case "pool":
            y = 85 * row + 80;
            break;

        default:
            y = 100 * row + 80;
    }

    return y;
}

class GriditemRect extends React.PureComponent<{
    scene: SceneType,
    type: GriditemType,
    row: number,
    col: number
}> {
    render() {
        let y = GetYpos(this.props.scene, this.props.row, this.props.col);

        return <div className={
            "griditem griditem-" + this.props.type
        } style={{
            left: 80 * this.props.col + 40,
            top: y
        }}></div>;
    }
}

export class IcePathRect extends React.PureComponent<{
    scene: SceneType,
    x: number,
    row: number
}> {
    render() {
        return <div className="ice-path" style={{
            width: 800 - this.props.x,
            height: ["pool", "fog"].indexOf(this.props.scene) === -1 ? 100 : 80,
            top: GetYpos(this.props.scene, this.props.row, (this.props.x - 40) / 80)
        }}></div>;
    }
}

export class SceneFrame extends React.PureComponent<{
    scene: Scene,
    status: EmulatorState,
    onPlayBtnClicked: (e: React.MouseEvent<HTMLButtonElement>) => void
}> {
    constructor(props) {
        super(props);
    }

    render() {
        let griditems = this.props.scene.griditems.map(p => {
            return <GriditemRect
                key={"griditem-" + p.id}
                scene={this.props.scene.type}
                row={p.row}
                col={p.col}
                type={p.type}/>;
        });

        let plantHitBoxes = this.props.scene.plants.map(p => {
            return <Rect
                key={p.type + '-hitbox-' + p.id}
                x={p.hit_box.x}
                y={p.hit_box.y}
                height={p.hit_box.height}
                width={p.hit_box.width}
                borderColor='#00FF00'/>;
        });

        let plantAttackBoxes = this.props.scene.plants.filter(p => [
            "potato_mine",
            "squash",
            "chomper",
            "puffshroom",
            "fumeshroom",
            "seashroom",
            "spikeweed",
            "spikerock",
            "gloomshroom"
        ].indexOf(p.type) !== -1).map(p => {
            return <Rect
                key={p.type + '-attackbox-' + p.id}
                x={p.attack_box.x}
                y={p.attack_box.y}
                height={p.attack_box.height}
                width={p.attack_box.width}
                borderColor='#FF0080'/>
        });

        let zombieHitBoxes = this.props.scene.zombies.map(z => {
            return <Rect
                key={z.type + '-hitbox-' + z.id}
                x={z.hit_box.x}
                y={z.hit_box.y}
                height={z.hit_box.height}
                width={z.hit_box.width}
                borderColor='#00FF00'/>
        });

        let zombieAttackBoxes = this.props.scene.zombies.map(z => {
            return <Rect
                key={z.type + '-attackbox-' + z.id}
                x={z.attack_box.x}
                y={z.attack_box.y}
                height={z.attack_box.height}
                width={z.attack_box.width}
                borderColor='#FF0000' />
        });

        let playButton = <button
            id="play-btn"
            type="button"
            onClick={this.props.onPlayBtnClicked}
            className={classnames("btn", {
                "btn-success": this.props.status.isPlaying,
                "btn-danger": !this.props.status.isPlaying
            })}>
            <i className={classnames("fas", {
                "fa-play": this.props.status.isPlaying,
                "fa-pause": !this.props.status.isPlaying
            })}></i>
        </button>;

        let projectileAttackBexes = this.props.scene.projectiles.map(p => {
            return <Rect
                key={p.type + '-' + p.id}
                x={p.attack_box.x}
                y={p.attack_box.y}
                width={p.attack_box.width}
                height={p.attack_box.height}
                borderColor="#0000FF" />;
        });

        let sunCard = <div className="card" style={{
            position: 'absolute',
            top: 0,
            left: 0
        }}>
            <div className="card-body">
                {"Sun: " + this.props.scene.sun.sun}
          </div>
        </div>;

        let n;
        if (["pool", "fog"].indexOf(this.props.scene.type) === -1) {
            n = 5;
        } else {
            n = 6;
        }

        let icePath = [];
        for (let i = 0; i < n; i++) {
            icePath.push(<IcePathRect key={'ice-path-' + i}
                scene={this.props.scene.type}
                x={this.props.scene.ice_path.x[i]} row={i} />);
        }

        return <div id="scene-frame" className={classnames({
            "background-day": this.props.scene.type == "day",
            "background-night": this.props.scene.type == "night",
            "background-roof": this.props.scene.type == "roof",
            "background-pool": this.props.scene.type == "pool",
            "background-fog": this.props.scene.type == "fog",
            "background-moon-night": this.props.scene.type == "moon_night"
        })}>{[
            plantHitBoxes,
            plantAttackBoxes,
            zombieHitBoxes,
            zombieAttackBoxes,
            griditems,
            icePath,
            projectileAttackBexes
        ]}{playButton}{sunCard}</div>;
    }
}