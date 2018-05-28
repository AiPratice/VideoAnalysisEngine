import env from "../build/env"

//前端配置信息
const config = {
    HOST: env === 'development' ? "http://127.0.0.1:1323" : "http://www.baidu.com",
    WS_HOST: env === 'development' ? "ws://127.0.0.1:1323/ws" : ""
}

export default config;