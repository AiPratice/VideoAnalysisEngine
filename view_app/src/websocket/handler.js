import { eventsType } from '../enum/event';

//websocket消息响应
export default (vueInstance,response) => {
    let respEvent = JSON.parse(response.data);
    console.log(respEvent);
    if (respEvent && respEvent.Type) {
        switch (respEvent.Type) {
            case eventsType.responseProcessVideo:
                //视频处理响应
                if(respEvent.StatusCode === 200){
                    vueInstance.$store.dispatch("getVideos");
                    vueInstance.$Message.info('视频处理完成');
                }else{
                    vueInstance.$Message.info('视频处理失败');
                }
                vueInstance.$store.commit('setIsLongTimeWorking',false);
                break;
            case eventsType.responseMergeIndex:
                 //合并索引响应
                if(respEvent.StatusCode === 200){
                    vueInstance.$store.dispatch("getVideos");
                    vueInstance.$Message.info('全局描述算子索引建立完成');
                }else{
                    vueInstance.$Message.info('全局描述算子索引建立失败');
                }
                vueInstance.$store.commit('setIsLongTimeWorking',false);
                break;
            case eventsType.responseTrainFaceClassifier:
                if(respEvent.StatusCode === 200){
                    vueInstance.$Message.info('人脸分类器训练完成');
                }else{
                    vueInstance.$Message.info('人脸分类器训练失败');
                }
                vueInstance.$store.commit('setIsLongTimeWorking',false);
                break;
            case eventsType.responseObjectDetectVideo:
                if(respEvent.StatusCode === 200){
                    vueInstance.$store.dispatch('getVideos');
                    vueInstance.$Message.info('视频帧物体检测完成');
                }else{
                    vueInstance.$Message.info('视频帧物体检测完成');
                }
                vueInstance.$store.commit('setIsLongTimeWorking',false);
                break;
            case eventsType.responseFaceRecognizeVideo:
                if(respEvent.StatusCode === 200){
                    vueInstance.$store.dispatch('getVideos');
                    vueInstance.$Message.info('视频帧人脸识别完成');
                }else{
                    vueInstance.$Message.info('视频帧人脸识别完成');
                }
                vueInstance.$store.commit('setIsLongTimeWorking',false);
                break;
            default:
                vueInstance.$Message.error("未知的websocket响应事件类型");
        }
    }
}