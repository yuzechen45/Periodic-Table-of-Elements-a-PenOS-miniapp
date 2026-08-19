<template>
  <div class="st-root">
    <div class="st-header">
      <div class="st-back" @click="goBack"><text class="st-back-text">返回</text></div>
      <text class="st-title">设置</text>
    </div>
    <scroller class="st-scroller" scroll-direction="vertical" :show-scrollbar="true">
      <div class="st-body">
        <div class="st-row">
          <text class="st-label">主页显示电负性</text>
          <fl-switch
            v-model="showNeg"
            :active-color="'#4a6ee0'"
            :inactive-color="'#39405a'"
            :color-button="'#e8eefc'"
            :width="46"
            :height="24"
            :button-margin="3"
            @change="onNegChange"
          ></fl-switch>
        </div>
        <div class="st-row">
          <text class="st-label">主页显示相对原子质量</text>
          <fl-switch
            v-model="showMass"
            :active-color="'#4a6ee0'"
            :inactive-color="'#39405a'"
            :color-button="'#e8eefc'"
            :width="46"
            :height="24"
            :button-margin="3"
            @change="onMassChange"
          ></fl-switch>
        </div>
        <div class="st-row">
          <text class="st-label">主页显示价电子</text>
          <fl-switch
            v-model="showVal"
            :active-color="'#4a6ee0'"
            :inactive-color="'#39405a'"
            :color-button="'#e8eefc'"
            :width="46"
            :height="24"
            :button-margin="3"
            @change="onValChange"
          ></fl-switch>
        </div>
        <div class="st-row">
          <text class="st-label">主页显示周期</text>
          <fl-switch
            v-model="showPeriod"
            :active-color="'#4a6ee0'"
            :inactive-color="'#39405a'"
            :color-button="'#e8eefc'"
            :width="46"
            :height="24"
            :button-margin="3"
            @change="onPeriodChange"
          ></fl-switch>
        </div>
        <div class="st-row">
          <text class="st-label">主页显示价电子排布式</text>
          <fl-switch
            v-model="showVconf"
            :active-color="'#4a6ee0'"
            :inactive-color="'#39405a'"
            :color-button="'#e8eefc'"
            :width="46"
            :height="24"
            :button-margin="3"
            @change="onVconfChange"
          ></fl-switch>
        </div>
        <text class="st-tip">设置自动保存，返回主页立即生效</text>
        <text class="st-copy">by yuze,welcome to baigei.cc</text>
      </div>
    </scroller>
  </div>
</template>

<script>
import { FlSwitch } from 'falcon-ui'

export default {
  name: 'settings',
  components: { FlSwitch },
  data() {
    return {
      showNeg: false,
      showMass: false,
      showVal: false,
      showPeriod: false,
      showVconf: false,
    }
  },
  methods: {
    onShow() {
      this.load()
    },
    async load() {
      var self = this
      var pairs = [
        ['pt-neg', function (d) { self.showNeg = d === '1' }],
        ['pt-mass', function (d) { self.showMass = d === '1' }],
        ['pt-val', function (d) { self.showVal = d === '1' }],
        ['pt-period', function (d) { self.showPeriod = d === '1' }],
        ['pt-vconf', function (d) { self.showVconf = d === '1' }],
      ]
      for (var i = 0; i < pairs.length; i++) {
        try {
          var res = await $falcon.jsapi.storage.getStorage({ key: pairs[i][0] })
          pairs[i][1](res && res.data)
        } catch (err) {}
      }
    },
    save(key, val) {
      var p = $falcon.jsapi.storage.setStorage({ key: key, data: val ? '1' : '0' })
      if (p && p.catch) p.catch(function () {})
    },
    onNegChange(val) {
      this.save('pt-neg', !!val)
    },
    onMassChange(val) {
      this.save('pt-mass', !!val)
    },
    onValChange(val) {
      this.save('pt-val', !!val)
    },
    onPeriodChange(val) {
      this.save('pt-period', !!val)
    },
    onVconfChange(val) {
      this.save('pt-vconf', !!val)
    },
    goBack() {
      this.$page.finish()
    },
  },
}
</script>

<style scoped>
.st-root {
  width: 100%;
  height: 100%;
  background-color: #0e1320;
  display: flex;
  flex-direction: column;
}

.st-header {
  height: 40px;
  display: flex;
  flex-direction: row;
  align-items: center;
  background-color: #161e30;
  padding-left: 8px;
}

.st-back {
  height: 26px;
  padding-left: 12px;
  padding-right: 12px;
  background-color: #2a3854;
  border-radius: 8px;
  display: flex;
  align-items: center;
  justify-content: center;
}

.st-back-text {
  color: #b9c8e8;
  font-size: 13px;
}

.st-title {
  flex: 1;
  color: #e8eefc;
  font-size: 16px;
  font-weight: bold;
  text-align: center;
  margin-right: 40px;
}

.st-scroller {
  flex: 1;
}

.st-body {
  padding: 10px;
}

.st-row {
  display: flex;
  flex-direction: row;
  align-items: center;
  height: 40px;
  background-color: #151d2e;
  border-radius: 8px;
  margin-bottom: 8px;
  padding-left: 12px;
  padding-right: 12px;
}

.st-label {
  flex: 1;
  color: #eef2fc;
  font-size: 14px;
}

.st-tip {
  color: #7d8aa8;
  font-size: 12px;
  margin-top: 10px;
  text-align: center;
  line-height: 18px;
}

.st-copy {
  color: #5c6a8c;
  font-size: 12px;
  margin-top: 40px;
  text-align: center;
  line-height: 18px;
}
</style>
