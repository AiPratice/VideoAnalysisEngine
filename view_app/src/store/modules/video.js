import Config from "../../config"
import Vue from 'vue';
import Util from '../../libs/util';

const video = {
    state:{
        videos:[], //视频列表
        videosMap:{} //视频映射
    },
    mutations:{
        addVideo(state,video){
            state.videos.push(video);
        },
        //批量设置videos
        setVideos(state,videos){
            state.videos = videos;
            state.videos.sort((video1,video2)=>{
                return video1.video_id - video2.video_id;
            });

            for(let i in videos) {
                let video = videos[i];
                state.videosMap[video.video_id] = {
                    name:video.name
                };
            }
        }
    },
    actions:{
        //从后端获取视频
        getVideos({commit}){
            Util.ajax.get("/videos/list")
                .then(function(response){
                    commit("setVideos",response.data);
                }).catch(function(error){
                    alert("无法从后端获取视频列表");
                });
        }
    }
};

export default video;